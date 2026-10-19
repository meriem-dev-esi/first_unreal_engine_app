#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FighterTypes.h"
#include "CombatPowerupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class URotatingMovementComponent;

UCLASS()
class FIRST_UNREAL_ENGINE_APP_API ACombatPowerupActor : public AActor
{
	GENERATED_BODY()

public:
	ACombatPowerupActor();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PowerupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerup")
	EPowerupType PowerupType = EPowerupType::HealthPack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerup")
	float ValueAmount = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerup")
	float DurationSeconds = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerup|FX")
	UParticleSystem* PickupEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerup|FX")
	USoundBase* PickupSound;
};
