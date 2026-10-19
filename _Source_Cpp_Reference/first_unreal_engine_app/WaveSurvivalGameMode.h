#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FighterTypes.h"
#include "WaveSurvivalGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveStateChangedSignature, EWaveState, NewState, int32, CurrentWave);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemiesRemainingSignature, int32, RemainingCount, int32, TotalWaveEnemies);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScoreUpdatedSignature, int32, TotalScore, int32, ComboStreak, float, Multiplier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatRankUpSignature, ECombatRank, NewRank);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossAlertSignature, FString, BossTitle);

UCLASS()
class FIRST_UNREAL_ENGINE_APP_API AWaveSurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWaveSurvivalGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Wave Controls
	UFUNCTION(BlueprintCallable, Category = "WaveSurvival")
	void StartWave(int32 WaveNum);

	UFUNCTION(BlueprintCallable, Category = "WaveSurvival")
	void EndCurrentWave();

	UFUNCTION(BlueprintCallable, Category = "WaveSurvival")
	void NotifyEnemyDefeated(AActor* DefeatedEnemy, AActor* Killer, int32 BaseScore = 100);

	UFUNCTION(BlueprintCallable, Category = "WaveSurvival")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "WaveSurvival")
	void TriggerPlayerDeath();

	// Powerup & Loot Spawning
	UFUNCTION(BlueprintCallable, Category = "WaveSurvival")
	void SpawnLootDrop(FVector Location);

	// Getters
	UFUNCTION(BlueprintPure, Category = "WaveSurvival")
	int32 GetCurrentWave() const { return CurrentWaveIndex; }

	UFUNCTION(BlueprintPure, Category = "WaveSurvival")
	int32 GetEnemiesAlive() const { return EnemiesAlive; }

	UFUNCTION(BlueprintPure, Category = "WaveSurvival")
	const FCombatScoreData& GetScoreData() const { return ScoreData; }

	UFUNCTION(BlueprintPure, Category = "WaveSurvival")
	EWaveState GetWaveState() const { return CurrentWaveState; }

public:
	// UI & Audio Broadcast Delegates
	UPROPERTY(BlueprintAssignable, Category = "WaveSurvival|Events")
	FOnWaveStateChangedSignature OnWaveStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "WaveSurvival|Events")
	FOnEnemiesRemainingSignature OnEnemiesRemainingChanged;

	UPROPERTY(BlueprintAssignable, Category = "WaveSurvival|Events")
	FOnScoreUpdatedSignature OnScoreUpdated;

	UPROPERTY(BlueprintAssignable, Category = "WaveSurvival|Events")
	FOnCombatRankUpSignature OnCombatRankUp;

	UPROPERTY(BlueprintAssignable, Category = "WaveSurvival|Events")
	FOnBossAlertSignature OnBossAlert;

protected:
	// Configuration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSurvival|Config")
	int32 MaxWaves = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSurvival|Config")
	float IntermissionSeconds = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSurvival|Config")
	float ComboDecayTimeSeconds = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSurvival|Config")
	TSubclassOf<AActor> MeleeEnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSurvival|Config")
	TSubclassOf<AActor> ShooterEnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSurvival|Config")
	TSubclassOf<AActor> BossEnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveSurvival|Config")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	// Runtime Tracking
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaveSurvival|State")
	int32 CurrentWaveIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaveSurvival|State")
	int32 TotalWaveEnemies = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaveSurvival|State")
	int32 EnemiesAlive = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaveSurvival|State")
	EWaveState CurrentWaveState = EWaveState::WaitingToStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaveSurvival|State")
	FCombatScoreData ScoreData;

	FTimerHandle IntermissionTimerHandle;
	FTimerHandle ComboDecayTimerHandle;
	FTimerHandle EnemySpawnBatchTimerHandle;

	// Internal Spawning Helpers
	void SpawnNextWaveBatch();
	void UpdateCombatRank();
	void ResetComboStreak();
	FVector FindRandomSpawnPoint();
};
