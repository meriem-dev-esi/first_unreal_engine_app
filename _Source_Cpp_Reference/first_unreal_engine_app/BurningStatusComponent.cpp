#include "BurningStatusComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

UBurningStatusComponent::UBurningStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBurningStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsOnFire)
	{
		BurnDurationRemaining -= DeltaTime;
		if (BurnDurationRemaining <= 0.0f)
		{
			Extinguish();
		}
	}
}

void UBurningStatusComponent::Ignite(float Duration, float DamagePerSecond, AActor* FireInstigator)
{
	BurnDurationRemaining = FMath::Max(BurnDurationRemaining, Duration);
	BurnDPS = DamagePerSecond;
	InstigatorActor = FireInstigator;

	if (!bIsOnFire)
	{
		bIsOnFire = true;
		OnBurnStateChanged.Broadcast(true);

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(BurnTickTimer, this, &UBurningStatusComponent::ApplyBurnTick, 0.5f, true);
		}
	}
}

void UBurningStatusComponent::ApplyBurnTick()
{
	if (!bIsOnFire) return;

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor) return;

	float TickDamage = BurnDPS * 0.5f;
	UGameplayStatics::ApplyDamage(
		OwnerActor,
		TickDamage,
		InstigatorActor ? InstigatorActor->GetInstigatorController() : nullptr,
		InstigatorActor,
		UDamageType::StaticClass()
	);
}

void UBurningStatusComponent::Extinguish()
{
	if (!bIsOnFire) return;

	bIsOnFire = false;
	BurnDurationRemaining = 0.0f;
	OnBurnStateChanged.Broadcast(false);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(BurnTickTimer);
	}
}
