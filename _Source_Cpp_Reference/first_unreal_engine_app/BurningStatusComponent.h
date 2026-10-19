#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BurningStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBurnStateChangedSignature, bool, bIsBurning);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class FIRST_UNREAL_ENGINE_APP_API UBurningStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBurningStatusComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Combat|Fire")
	void Ignite(float Duration = 5.0f, float DamagePerSecond = 15.0f, AActor* FireInstigator = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Combat|Fire")
	void Extinguish();

	UFUNCTION(BlueprintPure, Category = "Combat|Fire")
	bool IsBurning() const { return bIsOnFire; }

public:
	UPROPERTY(BlueprintAssignable, Category = "Combat|Fire|Events")
	FOnBurnStateChangedSignature OnBurnStateChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Fire")
	bool bIsOnFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Fire")
	float BurnDurationRemaining = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Fire")
	float BurnDPS = 15.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Fire")
	AActor* InstigatorActor = nullptr;

	FTimerHandle BurnTickTimer;

	void ApplyBurnTick();
};
