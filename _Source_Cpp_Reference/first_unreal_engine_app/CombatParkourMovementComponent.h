#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FighterTypes.h"
#include "CombatParkourMovementComponent.generated.h"

class ACharacter;
class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnParkourStateChangedSignature, ECombatParkourState, NewState);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class FIRST_UNREAL_ENGINE_APP_API UCombatParkourMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatParkourMovementComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Parkour Actions
	UFUNCTION(BlueprintCallable, Category = "Combat|Parkour")
	bool TryDoubleJump();

	UFUNCTION(BlueprintCallable, Category = "Combat|Parkour")
	bool TryAirDash(FVector InputDirection);

	UFUNCTION(BlueprintCallable, Category = "Combat|Parkour")
	bool StartSlide();

	UFUNCTION(BlueprintCallable, Category = "Combat|Parkour")
	void StopSlide();

	UFUNCTION(BlueprintPure, Category = "Combat|Parkour")
	ECombatParkourState GetParkourState() const { return CurrentParkourState; }

	UFUNCTION(BlueprintPure, Category = "Combat|Parkour")
	bool CanAirDash() const { return bCanAirDash; }

public:
	UPROPERTY(BlueprintAssignable, Category = "Combat|Parkour|Events")
	FOnParkourStateChangedSignature OnParkourStateChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Parkour")
	float DoubleJumpForce = 750.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Parkour")
	float AirDashImpulse = 1600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Parkour")
	float SlideInitialBoost = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Parkour")
	float SlideDurationSeconds = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Parkour")
	ECombatParkourState CurrentParkourState = ECombatParkourState::Grounded;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Parkour")
	bool bCanDoubleJump = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Parkour")
	bool bCanAirDash = true;

	ACharacter* CharacterOwner = nullptr;
	UCharacterMovementComponent* MovementComp = nullptr;
	FTimerHandle SlideTimerHandle;
};
