#include "TacticalTimeDilationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

UTacticalTimeDilationComponent::UTacticalTimeDilationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = false;
}

void UTacticalTimeDilationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsBulletTimeActive)
	{
		// Drain focus energy during bullet time
		CurrentFocusEnergy = FMath::Clamp(CurrentFocusEnergy - (DrainRatePerSecond * DeltaTime), 0.0f, MaxFocusEnergy);
		OnFocusEnergyChanged.Broadcast(CurrentFocusEnergy, MaxFocusEnergy);

		if (CurrentFocusEnergy <= 0.0f)
		{
			DeactivateBulletTime();
		}
	}
	else
	{
		// Slowly regenerate focus energy when not in slow motion
		if (CurrentFocusEnergy < MaxFocusEnergy)
		{
			CurrentFocusEnergy = FMath::Clamp(CurrentFocusEnergy + (RegenRatePerSecond * DeltaTime), 0.0f, MaxFocusEnergy);
			OnFocusEnergyChanged.Broadcast(CurrentFocusEnergy, MaxFocusEnergy);
		}
	}
}

void UTacticalTimeDilationComponent::ToggleBulletTime()
{
	if (bIsBulletTimeActive)
	{
		DeactivateBulletTime();
	}
	else
	{
		ActivateBulletTime();
	}
}

void UTacticalTimeDilationComponent::ActivateBulletTime()
{
	if (bIsBulletTimeActive || CurrentFocusEnergy < MinimumEnergyToActivate)
	{
		return;
	}

	bIsBulletTimeActive = true;

	// Slow down the entire world
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowMoDilation);

	// Keep the player moving at normal / superior relative speed!
	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->CustomTimeDilation = 1.0f / SlowMoDilation;
	}

	OnBulletTimeToggled.Broadcast(true, SlowMoDilation);
}

void UTacticalTimeDilationComponent::DeactivateBulletTime()
{
	if (!bIsBulletTimeActive)
	{
		return;
	}

	bIsBulletTimeActive = false;

	// Restore normal world time
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->CustomTimeDilation = 1.0f;
	}

	OnBulletTimeToggled.Broadcast(false, 1.0f);
}

void UTacticalTimeDilationComponent::AddFocusEnergy(float Amount)
{
	CurrentFocusEnergy = FMath::Clamp(CurrentFocusEnergy + Amount, 0.0f, MaxFocusEnergy);
	OnFocusEnergyChanged.Broadcast(CurrentFocusEnergy, MaxFocusEnergy);
}
