#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FighterTypes.h"
#include "CombatInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSwitchedSignature, EWeaponSlot, NewSlot, const FWeaponSlotData&, WeaponData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, int32, CurrentAmmo, int32, ReserveAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReloadStartedSignature, float, ReloadDurationSeconds);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class FIRST_UNREAL_ENGINE_APP_API UCombatInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatInventoryComponent();

	virtual void BeginPlay() override;

	// Weapon Slot Selection
	UFUNCTION(BlueprintCallable, Category = "Combat|Inventory")
	void SwitchToSlot(EWeaponSlot NewSlot);

	UFUNCTION(BlueprintCallable, Category = "Combat|Inventory")
	void NextWeapon();

	UFUNCTION(BlueprintCallable, Category = "Combat|Inventory")
	void PreviousWeapon();

	// Firing & Reloading
	UFUNCTION(BlueprintCallable, Category = "Combat|Inventory")
	bool ConsumeAmmo(int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Combat|Inventory")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Combat|Inventory")
	void AddReserveAmmo(EWeaponSlot Slot, int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Combat|Inventory")
	EWeaponSlot GetCurrentSlot() const { return CurrentSlot; }

	UFUNCTION(BlueprintPure, Category = "Combat|Inventory")
	const FWeaponSlotData& GetCurrentWeaponData() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Inventory")
	bool CanFire() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Combat|Inventory|Events")
	FOnWeaponSwitchedSignature OnWeaponSwitched;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Inventory|Events")
	FOnAmmoChangedSignature OnAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category = "Combat|Inventory|Events")
	FOnReloadStartedSignature OnReloadStarted;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Inventory")
	TMap<EWeaponSlot, FWeaponSlotData> WeaponArsenal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Inventory")
	EWeaponSlot CurrentSlot = EWeaponSlot::Primary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Inventory")
	bool bIsReloading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Inventory")
	float ReloadTimeSeconds = 1.8f;

	FTimerHandle ReloadTimerHandle;

	void FinishReload();
};
