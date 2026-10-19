#include "ExplosiveBarrelActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

AExplosiveBarrelActor::AExplosiveBarrelActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	RootComponent = BarrelMesh;
	BarrelMesh->SetSimulatePhysics(true);
	BarrelMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
}

void AExplosiveBarrelActor::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

float AExplosiveBarrelActor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bHasExploded) return 0.0f;

	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.0f)
	{
		Explode();
	}

	return DamageAmount;
}

void AExplosiveBarrelActor::Explode()
{
	if (bHasExploded) return;
	bHasExploded = true;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector ExplodeLoc = GetActorLocation();

	// Play Visual & Sound effects
	if (ExplosionEmitter)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEmitter, ExplodeLoc, FRotator::ZeroRotator, FVector(2.0f));
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(World, ExplosionSound, ExplodeLoc);
	}

	// Apply Radial Area Damage to all surrounding enemies and player
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	UGameplayStatics::ApplyRadialDamageWithFalloff(
		World,
		ExplosionBaseDamage,
		20.0f,
		ExplodeLoc,
		ExplosionRadius * 0.4f,
		ExplosionRadius,
		1.0f,
		UDamageType::StaticClass(),
		IgnoreActors,
		this,
		GetInstigatorController()
	);

	// Apply Physics Impulse to launch nearby debris & ragdolls
	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(ExplosionRadius);
	bool bHit = World->SweepMultiByChannel(
		HitResults,
		ExplodeLoc,
		ExplodeLoc + FVector(0.f, 0.f, 10.f),
		FQuat::Identity,
		ECC_PhysicsBody,
		SphereShape
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (UPrimitiveComponent* PrimComp = Hit.GetComponent())
			{
				if (PrimComp->IsSimulatingPhysics())
				{
					PrimComp->AddRadialImpulse(ExplodeLoc, ExplosionRadius, ExplosionImpulse, ERadialImpulseFalloff::RIF_Linear, true);
				}
			}
		}
	}

	Destroy();
}
