#include "WaveSurvivalGameMode.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

AWaveSurvivalGameMode::AWaveSurvivalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWaveSurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	ScoreData.CurrentRank = ECombatRank::Rank_D;
	ScoreData.ScoreMultiplier = 1.0f;

	// Start Wave 1 after a short 3-second ready countdown
	GetWorldTimerManager().SetTimer(IntermissionTimerHandle, [this]()
	{
		StartWave(1);
	}, 3.0f, false);
}

void AWaveSurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaveSurvivalGameMode::StartWave(int32 WaveNum)
{
	CurrentWaveIndex = WaveNum;
	GetWorldTimerManager().ClearTimer(IntermissionTimerHandle);

	bool bIsBoss = (CurrentWaveIndex % 5 == 0);
	CurrentWaveState = bIsBoss ? EWaveState::BossWave : EWaveState::WaveInProgress;

	// Scale enemies per wave
	int32 MeleeCount = 2 + (CurrentWaveIndex * 2);
	int32 ShooterCount = 1 + (CurrentWaveIndex * 1);
	if (bIsBoss)
	{
		MeleeCount += 2;
		ShooterCount += 2;
	}

	TotalWaveEnemies = MeleeCount + ShooterCount + (bIsBoss ? 1 : 0);
	EnemiesAlive = TotalWaveEnemies;

	OnWaveStateChanged.Broadcast(CurrentWaveState, CurrentWaveIndex);
	OnEnemiesRemainingChanged.Broadcast(EnemiesAlive, TotalWaveEnemies);

	if (bIsBoss)
	{
		FString BossTitle = FString::Printf(TEXT("ANNIHILATOR PRIME - WAVE %d BOSS"), CurrentWaveIndex);
		OnBossAlert.Broadcast(BossTitle);
	}

	// Spawn Enemies in staggered batches to keep performance smooth
	SpawnNextWaveBatch();
}

void AWaveSurvivalGameMode::SpawnNextWaveBatch()
{
	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn Melee and Shooter enemies at surrounding arena locations
	int32 TotalToSpawn = EnemiesAlive;
	for (int32 i = 0; i < TotalToSpawn; ++i)
	{
		FVector SpawnLoc = FindRandomSpawnPoint();
		FRotator SpawnRot = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);

		TSubclassOf<AActor> ClassToSpawn = (i % 2 == 0 && MeleeEnemyClass) ? MeleeEnemyClass : ShooterEnemyClass;
		if (ClassToSpawn)
		{
			AActor* SpawnedActor = World->SpawnActor<AActor>(ClassToSpawn, SpawnLoc, SpawnRot, SpawnParams);
			if (ACharacter* Char = Cast<ACharacter>(SpawnedActor))
			{
				// Scale speed and responsiveness with wave progression
				if (UCharacterMovementComponent* MoveComp = Char->GetCharacterMovement())
				{
					MoveComp->MaxWalkSpeed *= (1.0f + (CurrentWaveIndex * 0.05f));
				}
			}
		}
	}

	// Spawn Boss if boss wave
	if (CurrentWaveState == EWaveState::BossWave && BossEnemyClass)
	{
		FVector BossLoc = FindRandomSpawnPoint() + FVector(0.f, 0.f, 100.f);
		AActor* BossActor = World->SpawnActor<AActor>(BossEnemyClass, BossLoc, FRotator::ZeroRotator, SpawnParams);
		if (BossActor)
		{
			BossActor->SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));
		}
	}
}

void AWaveSurvivalGameMode::NotifyEnemyDefeated(AActor* DefeatedEnemy, AActor* Killer, int32 BaseScore)
{
	EnemiesAlive = FMath::Max(0, EnemiesAlive - 1);
	ScoreData.TotalKills++;
	ScoreData.CurrentCombo++;

	if (ScoreData.CurrentCombo > ScoreData.MaxComboStreak)
	{
		ScoreData.MaxComboStreak = ScoreData.CurrentCombo;
	}

	// Calculate Multiplier: Combo 1-4 = x1, 5-9 = x1.5, 10-14 = x2, 15+ = x3
	if (ScoreData.CurrentCombo >= 15) ScoreData.ScoreMultiplier = 3.0f;
	else if (ScoreData.CurrentCombo >= 10) ScoreData.ScoreMultiplier = 2.0f;
	else if (ScoreData.CurrentCombo >= 5) ScoreData.ScoreMultiplier = 1.5f;
	else ScoreData.ScoreMultiplier = 1.0f;

	int32 PointsEarned = FMath::RoundToInt(BaseScore * ScoreData.ScoreMultiplier);
	AddScore(PointsEarned);

	// Reset combo decay timer
	GetWorldTimerManager().ClearTimer(ComboDecayTimerHandle);
	GetWorldTimerManager().SetTimer(ComboDecayTimerHandle, this, &AWaveSurvivalGameMode::ResetComboStreak, ComboDecayTimeSeconds, false);

	// Chance to drop power-up at enemy location (25% chance)
	if (DefeatedEnemy && FMath::FRand() < 0.25f)
	{
		SpawnLootDrop(DefeatedEnemy->GetActorLocation());
	}

	OnEnemiesRemainingChanged.Broadcast(EnemiesAlive, TotalWaveEnemies);

	// Check if wave is cleared
	if (EnemiesAlive <= 0)
	{
		EndCurrentWave();
	}
}

void AWaveSurvivalGameMode::AddScore(int32 Amount)
{
	ScoreData.TotalScore += Amount;
	UpdateCombatRank();
	OnScoreUpdated.Broadcast(ScoreData.TotalScore, ScoreData.CurrentCombo, ScoreData.ScoreMultiplier);
}

void AWaveSurvivalGameMode::UpdateCombatRank()
{
	ECombatRank NewRank = ECombatRank::Rank_D;
	if (ScoreData.TotalScore >= 25000) NewRank = ECombatRank::Rank_SSS;
	else if (ScoreData.TotalScore >= 15000) NewRank = ECombatRank::Rank_S;
	else if (ScoreData.TotalScore >= 8000) NewRank = ECombatRank::Rank_A;
	else if (ScoreData.TotalScore >= 4000) NewRank = ECombatRank::Rank_B;
	else if (ScoreData.TotalScore >= 1500) NewRank = ECombatRank::Rank_C;

	if (NewRank != ScoreData.CurrentRank)
	{
		ScoreData.CurrentRank = NewRank;
		OnCombatRankUp.Broadcast(ScoreData.CurrentRank);
	}
}

void AWaveSurvivalGameMode::ResetComboStreak()
{
	ScoreData.CurrentCombo = 0;
	ScoreData.ScoreMultiplier = 1.0f;
	OnScoreUpdated.Broadcast(ScoreData.TotalScore, ScoreData.CurrentCombo, ScoreData.ScoreMultiplier);
}

void AWaveSurvivalGameMode::EndCurrentWave()
{
	int32 WaveBonus = 500 * CurrentWaveIndex;
	AddScore(WaveBonus);

	if (CurrentWaveIndex >= MaxWaves)
	{
		CurrentWaveState = EWaveState::Victory;
		OnWaveStateChanged.Broadcast(CurrentWaveState, CurrentWaveIndex);
		return;
	}

	CurrentWaveState = EWaveState::Intermission;
	OnWaveStateChanged.Broadcast(CurrentWaveState, CurrentWaveIndex);

	// Intermission countdown before next wave begins
	GetWorldTimerManager().SetTimer(IntermissionTimerHandle, [this]()
	{
		StartWave(CurrentWaveIndex + 1);
	}, IntermissionSeconds, false);
}

void AWaveSurvivalGameMode::TriggerPlayerDeath()
{
	CurrentWaveState = EWaveState::GameOver;
	GetWorldTimerManager().ClearTimer(IntermissionTimerHandle);
	GetWorldTimerManager().ClearTimer(ComboDecayTimerHandle);
	OnWaveStateChanged.Broadcast(CurrentWaveState, CurrentWaveIndex);
}

void AWaveSurvivalGameMode::SpawnLootDrop(FVector Location)
{
	if (PowerupClasses.Num() == 0 || !GetWorld()) return;

	int32 RandomIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
	TSubclassOf<AActor> SelectedPowerup = PowerupClasses[RandomIndex];

	if (SelectedPowerup)
	{
		FVector SpawnLoc = Location + FVector(0.f, 0.f, 50.f);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(SelectedPowerup, SpawnLoc, FRotator::ZeroRotator, SpawnParams);
	}
}

FVector AWaveSurvivalGameMode::FindRandomSpawnPoint()
{
	FVector CenterLoc = FVector::ZeroVector;
	float Radius = 2000.0f;

	// Spread spawns across arena perimeter
	float Angle = FMath::RandRange(0.f, 2.f * PI);
	float Dist = FMath::RandRange(600.f, Radius);

	float X = FMath::Cos(Angle) * Dist;
	float Y = FMath::Sin(Angle) * Dist;

	return FVector(X, Y, 50.0f);
}
