#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FighterTypes.h"
#include "FighterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedSignature, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatStateChangedSignature, EFighterCombatState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockOnTargetChangedSignature, AActor*, TargetActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUltimateTriggeredSignature, float, CooldownRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUltimateReadySignature);

UCLASS()
class FIRST_UNREAL_ENGINE_APP_API AFighterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFighterCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Combat Actions (Callable in C++ and Blueprints)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void LightAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void HeavyAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SuperAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartBlock();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopBlock();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ToggleLockOn();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Dodge();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool PerformMeleeHitCheck(float Damage, float Radius = 60.0f, float ForwardOffset = 100.0f);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ResetCombo();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCombatState(EFighterCombatState NewState);

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool CanUseUltimate() const { return bCanUseUltimate; }

	// Getters
	UFUNCTION(BlueprintPure, Category = "Combat")
	EFighterCombatState GetCombatState() const { return CombatState; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetHealthPercent() const { return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetStaminaPercent() const { return MaxStamina > 0.f ? CurrentStamina / MaxStamina : 0.f; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsTargetLocked() const { return bIsLockedOn && TargetActor != nullptr; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	AActor* GetLockedTarget() const { return TargetActor; }

public:
	// Event Delegates for UI and Blueprint Effects
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnAttributeChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnCombatStateChangedSignature OnCombatStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnLockOnTargetChangedSignature OnLockOnTargetChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnUltimateTriggeredSignature OnUltimateTriggered;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnUltimateReadySignature OnUltimateReady;

protected:
	virtual void BeginPlay() override;

	// Camera setup
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	// Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxStamina = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float StaminaRegenRate = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float LightAttackStaminaCost = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float HeavyAttackStaminaCost = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float SuperAttackStaminaCost = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float BlockDamageReduction = 0.75f;

	// Super Attack / Ultimate Ability Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Ultimate")
	float UltimateDamage = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Ultimate")
	float UltimateRadius = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Ultimate")
	float UltimateLaunchForce = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Ultimate")
	float UltimateCooldownSeconds = 6.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Ultimate")
	bool bCanUseUltimate = true;

	FTimerHandle UltimateCooldownTimer;

	UFUNCTION()
	void ResetUltimateCooldown();

	// Combat Configuration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	EFighterCombatState CombatState = EFighterCombatState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 ComboIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	TArray<UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	TArray<UAnimMontage*> HeavyAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Animation")
	UAnimMontage* DodgeMontage;

	// Lock-On System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|LockOn")
	float LockOnRange = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|LockOn")
	bool bIsLockedOn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|LockOn")
	AActor* TargetActor = nullptr;

	FTimerHandle ComboResetTimer;

	void UpdateTargetLock(float DeltaTime);
	AActor* FindBestLockOnTarget();
	void Die();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
