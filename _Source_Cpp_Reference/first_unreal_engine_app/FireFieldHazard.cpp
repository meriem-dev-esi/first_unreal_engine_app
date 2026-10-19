#include "FireFieldHazard.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BurningStatusComponent.h"
#include "TimerManager.h"

AFireFieldHazard::AFireFieldHazard()
{
	PrimaryActorTick.bCanEverTick = false;

	FireRadiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FireRadiusSphere"));
	RootComponent = FireRadiusSphere;
	FireRadiusSphere->SetSphereRadius(FireRadius);
	FireRadiusSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AFireFieldHazard::BeginPlay()
{
	Super::BeginPlay();

	FireRadiusSphere->OnComponentBeginOverlap.AddDynamic(this, &AFireFieldHazard::OnOverlapBegin);

	UWorld* World = GetWorld();
	if (World)
	{
		// Spawn fiery visual particles and sound
		if (FireEmitter)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, FireEmitter, GetActorLocation(), FRotator::ZeroRotator, FVector(2.5f));
		}
		if (BurningLoopSound)
		{
			UGameplayStatics::PlaySoundAtLocation(World, BurningLoopSound, GetActorLocation());
		}

		// Pulse damage every 0.5s
		World->GetTimerManager().SetTimer(PulseDamageTimer, this, &AFireFieldHazard::ApplyFirePulse, 0.5f, true);

		// Self-extinguish after lifetime
		SetLifeSpan(LifetimeSeconds);
	}
}

void AFireFieldHazard::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	// Ignite target if it has a burning status component
	if (UBurningStatusComponent* BurnComp = OtherActor->FindComponentByClass<UBurningStatusComponent>())
	{
		BurnComp->Ignite(IgniteDuration, DamagePerSecond, GetInstigator());
	}
}

void AFireFieldHazard::ApplyFirePulse()
{
	UWorld* World = GetWorld();
	if (!World) return;

	TArray<AActor*> Overlaps;
	FireRadiusSphere->GetOverlappingActors(Overlaps);

	for (AActor* Target : Overlaps)
	{
		if (Target && Target != this)
		{
			UGameplayStatics::ApplyDamage(
				Target,
				DamagePerSecond * 0.5f,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);

			// Apply burn status
			if (UBurningStatusComponent* BurnComp = Target->FindComponentByClass<UBurningStatusComponent>())
			{
				BurnComp->Ignite(IgniteDuration, DamagePerSecond, GetInstigator());
			}
		}
	}
}
