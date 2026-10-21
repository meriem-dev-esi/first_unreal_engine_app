#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TacticalTimeDilationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBulletTimeToggledSignature, bool, bIsActive, float, DilationAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFocusEnergyChangedSignature, float, CurrentFocus, float, MaxFocus);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class FIRST_UNREAL_ENGINE_APP_API UTacticalTimeDilationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTacticalTimeDilationComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Combat|BulletTime")
	void ToggleBulletTime();

	UFUNCTION(BlueprintCallable, Category = "Combat|BulletTime")
	void ActivateBulletTime();

	UFUNCTION(BlueprintCallable, Category = "Combat|BulletTime")
	void DeactivateBulletTime();

	UFUNCTION(BlueprintCallable, Category = "Combat|BulletTime")
	void AddFocusEnergy(float Amount);

	UFUNCTION(BlueprintPure, Category = "Combat|BulletTime")
	bool IsBulletTimeActive() const { return bIsBulletTimeActive; }

	UFUNCTION(BlueprintPure, Category = "Combat|BulletTime")
	float GetFocusPercent() const { return MaxFocusEnergy > 0.0f ? CurrentFocusEnergy / MaxFocusEnergy : 0.0f; }

public:
	UPROPERTY(BlueprintAssignable, Category = "Combat|BulletTime|Events")
	FOnBulletTimeToggledSignature OnBulletTimeToggled;

	UPROPERTY(BlueprintAssignable, Category = "Combat|BulletTime|Events")
	FOnFocusEnergyChangedSignature OnFocusEnergyChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|BulletTime")
	float SlowMoDilation = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|BulletTime")
	float MaxFocusEnergy = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|BulletTime")
	float CurrentFocusEnergy = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|BulletTime")
	float DrainRatePerSecond = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|BulletTime")
	float RegenRatePerSecond = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|BulletTime")
	float MinimumEnergyToActivate = 15.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|BulletTime")
	bool bIsBulletTimeActive = false;
};
