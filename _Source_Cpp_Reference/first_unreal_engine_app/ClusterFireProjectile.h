#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClusterFireProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class FIRST_UNREAL_ENGINE_APP_API AClusterFireProjectile : public AActor
{
	GENERATED_BODY()

public:
	AClusterFireProjectile();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Combat|Explosives")
	void Detonate();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Explosives")
	float MainBlastDamage = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Explosives")
	float MainBlastRadius = 550.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Explosives")
	int32 ClusterBombletCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Explosives")
	float ClusterSpreadVelocity = 650.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX")
	UParticleSystem* ExplosionEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|SFX")
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX")
	TSubclassOf<AActor> FireHazardFieldClass;

	bool bDetonated = false;
};
