#include "ClusterFireProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AClusterFireProjectile::AClusterFireProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(18.0f);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = CollisionComp;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2200.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.25f;
}

void AClusterFireProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentHit.AddDynamic(this, &AClusterFireProjectile::OnHit);
	SetLifeSpan(4.0f);
}

void AClusterFireProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetInstigator())
	{
		Detonate();
	}
}

void AClusterFireProjectile::Detonate()
{
	if (bDetonated) return;
	bDetonated = true;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector DetonationLoc = GetActorLocation();

	// Primary Massive Fire Blast
	if (ExplosionEmitter)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEmitter, DetonationLoc, FRotator::ZeroRotator, FVector(3.0f));
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(World, ExplosionSound, DetonationLoc);
	}

	// Apply Radial Damage to surrounding area
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(
		World,
		MainBlastDamage,
		DetonationLoc,
		MainBlastRadius,
		UDamageType::StaticClass(),
		IgnoredActors,
		this,
		GetInstigatorController(),
		true
	);

	// Spawn Persistent Napalm Fire Field at detonation point
	if (FireHazardFieldClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		World->SpawnActor<AActor>(FireHazardFieldClass, DetonationLoc, FRotator::ZeroRotator, SpawnParams);
	}

	// Scatter Secondary Fire Cluster Bomblets in an explosive radial starburst
	for (int32 i = 0; i < ClusterBombletCount; ++i)
	{
		float Angle = (2.0f * PI / ClusterBombletCount) * i;
		FVector ScatterDir = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.5f).GetSafeNormal();
		FVector SubDetonateLoc = DetonationLoc + (ScatterDir * 250.0f);

		if (ExplosionEmitter)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEmitter, SubDetonateLoc, FRotator::ZeroRotator, FVector(1.5f));
		}
		UGameplayStatics::ApplyRadialDamage(
			World,
			MainBlastDamage * 0.5f,
			SubDetonateLoc,
			MainBlastRadius * 0.6f,
			UDamageType::StaticClass(),
			IgnoredActors,
			this,
			GetInstigatorController(),
			true
		);
	}

	Destroy();
}
