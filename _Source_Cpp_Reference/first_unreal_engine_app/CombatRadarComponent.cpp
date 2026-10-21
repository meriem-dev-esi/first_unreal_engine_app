#include "CombatRadarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "ExplosiveBarrelActor.h"
#include "CombatPowerupActor.h"

UCombatRadarComponent::UCombatRadarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatRadarComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(PeriodicScanTimer, this, &UCombatRadarComponent::PerformRadarScan, ScanIntervalSeconds, true);
	}
}

void UCombatRadarComponent::PerformRadarScan()
{
	AActor* OwnerActor = GetOwner();
	UWorld* World = GetWorld();
	if (!OwnerActor || !World) return;

	CachedPings.Empty();
	const FVector PlayerLoc = OwnerActor->GetActorLocation();

	// 1. Scan for Characters / AI Enemies
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(World, ACharacter::StaticClass(), FoundCharacters);

	for (AActor* Actor : FoundCharacters)
	{
		if (Actor && Actor != OwnerActor)
		{
			float Dist = FVector::Dist(PlayerLoc, Actor->GetActorLocation());
			if (Dist <= RadarScanRadius)
			{
				FRadarPingData Ping;
				Ping.WorldLocation = Actor->GetActorLocation();
				Ping.DistanceToPlayer = Dist;
				Ping.bIsHostile = true;

				// Distinguish Boss by scale or tag
				if (Actor->GetActorScale3D().X > 1.2f)
				{
					Ping.TargetType = ERadarTargetType::BossEnemy;
				}
				else
				{
					Ping.TargetType = ERadarTargetType::ShooterEnemy;
				}

				CachedPings.Add(Ping);
			}
		}
	}

	// 2. Scan for Explosive Barrels
	TArray<AActor*> FoundBarrels;
	UGameplayStatics::GetAllActorsOfClass(World, AExplosiveBarrelActor::StaticClass(), FoundBarrels);
	for (AActor* Barrel : FoundBarrels)
	{
		if (Barrel)
		{
			float Dist = FVector::Dist(PlayerLoc, Barrel->GetActorLocation());
			if (Dist <= RadarScanRadius)
			{
				FRadarPingData Ping;
				Ping.WorldLocation = Barrel->GetActorLocation();
				Ping.DistanceToPlayer = Dist;
				Ping.TargetType = ERadarTargetType::ExplosiveHazard;
				Ping.bIsHostile = false;
				CachedPings.Add(Ping);
			}
		}
	}

	// 3. Scan for Collectible Powerups
	TArray<AActor*> FoundPowerups;
	UGameplayStatics::GetAllActorsOfClass(World, ACombatPowerupActor::StaticClass(), FoundPowerups);
	for (AActor* Powerup : FoundPowerups)
	{
		if (Powerup)
		{
			float Dist = FVector::Dist(PlayerLoc, Powerup->GetActorLocation());
			if (Dist <= RadarScanRadius)
			{
				FRadarPingData Ping;
				Ping.WorldLocation = Powerup->GetActorLocation();
				Ping.DistanceToPlayer = Dist;
				Ping.TargetType = ERadarTargetType::Powerup;
				Ping.bIsHostile = false;
				CachedPings.Add(Ping);
			}
		}
	}

	OnRadarScanCompleted.Broadcast(CachedPings);
}

int32 UCombatRadarComponent::GetHostileCount() const
{
	int32 Count = 0;
	for (const FRadarPingData& Ping : CachedPings)
	{
		if (Ping.bIsHostile) Count++;
	}
	return Count;
}
