#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireFieldHazard.generated.h"

class USphereComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class FIRST_UNREAL_ENGINE_APP_API AFireFieldHazard : public AActor
{
	GENERATED_BODY()

public:
	AFireFieldHazard();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* FireRadiusSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Fire")
	float FireRadius = 450.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Fire")
	float LifetimeSeconds = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Fire")
	float DamagePerSecond = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Fire")
	float IgniteDuration = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|FX")
	UParticleSystem* FireEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|FX")
	USoundBase* BurningLoopSound;

	FTimerHandle PulseDamageTimer;
	TArray<AActor*> OverlappingActors;

	void ApplyFirePulse();
};
