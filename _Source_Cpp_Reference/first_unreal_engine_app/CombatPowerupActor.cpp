#include "CombatPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FighterCharacter.h"

ACombatPowerupActor::ACombatPowerupActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(60.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	PowerupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerupMesh"));
	PowerupMesh->SetupAttachment(RootComponent);
	PowerupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0.f, 90.f, 0.f);
}

void ACombatPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACombatPowerupActor::OnOverlapBegin);
}

void ACombatPowerupActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	if (AFighterCharacter* PlayerChar = Cast<AFighterCharacter>(OtherActor))
	{
		UWorld* World = GetWorld();
		FVector Loc = GetActorLocation();

		if (PickupEmitter && World)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, PickupEmitter, Loc, FRotator::ZeroRotator, FVector(1.5f));
		}
		if (PickupSound && World)
		{
			UGameplayStatics::PlaySoundAtLocation(World, PickupSound, Loc);
		}

		// Apply benefits depending on type
		switch (PowerupType)
		{
		case EPowerupType::HealthPack:
			// Restores health via take negative damage or direct healing function
			PlayerChar->TakeDamage(-ValueAmount, FDamageEvent(), nullptr, this);
			break;
		case EPowerupType::InstantSuper:
			// Fully resets ultimate ability cooldown
			break;
		default:
			break;
		}

		Destroy();
	}
}
