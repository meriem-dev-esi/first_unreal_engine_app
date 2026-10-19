#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrelActor.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class FIRST_UNREAL_ENGINE_APP_API AExplosiveBarrelActor : public AActor
{
	GENERATED_BODY()

public:
	AExplosiveBarrelActor();

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Combat|Hazard")
	void Explode();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BarrelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Hazard")
	float MaxHealth = 25.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Hazard")
	float CurrentHealth = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Hazard")
	float ExplosionRadius = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Hazard")
	float ExplosionBaseDamage = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Hazard")
	float ExplosionImpulse = 2500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Hazard")
	bool bHasExploded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|VFX")
	UParticleSystem* ExplosionEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|SFX")
	USoundBase* ExplosionSound;
};
