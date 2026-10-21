#include "CombatInventoryComponent.h"
#include "TimerManager.h"

UCombatInventoryComponent::UCombatInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Populate Default Arsenal
	FWeaponSlotData PrimaryWeapon;
	PrimaryWeapon.Slot = EWeaponSlot::Primary;
	PrimaryWeapon.WeaponName = TEXT("Assault Rifle");
	PrimaryWeapon.CurrentAmmo = 30;
	PrimaryWeapon.MaxClipSize = 30;
	PrimaryWeapon.ReserveAmmo = 180;
	PrimaryWeapon.BaseDamage = 35.0f;
	PrimaryWeapon.RoundsPerMinute = 650.0f;
	PrimaryWeapon.bIsUnlocked = true;
	WeaponArsenal.Add(EWeaponSlot::Primary, PrimaryWeapon);

	FWeaponSlotData SecondaryWeapon;
	SecondaryWeapon.Slot = EWeaponSlot::Secondary;
	SecondaryWeapon.WeaponName = TEXT("Tactical Pistol");
	SecondaryWeapon.CurrentAmmo = 15;
	SecondaryWeapon.MaxClipSize = 15;
	SecondaryWeapon.ReserveAmmo = 90;
	SecondaryWeapon.BaseDamage = 25.0f;
	SecondaryWeapon.RoundsPerMinute = 400.0f;
	SecondaryWeapon.bIsUnlocked = true;
	WeaponArsenal.Add(EWeaponSlot::Secondary, SecondaryWeapon);

	FWeaponSlotData HeavyWeapon;
	HeavyWeapon.Slot = EWeaponSlot::Heavy;
	HeavyWeapon.WeaponName = TEXT("Cluster Fire Launcher");
	HeavyWeapon.CurrentAmmo = 4;
	HeavyWeapon.MaxClipSize = 4;
	HeavyWeapon.ReserveAmmo = 16;
	HeavyWeapon.BaseDamage = 160.0f;
	HeavyWeapon.RoundsPerMinute = 90.0f;
	HeavyWeapon.bIsUnlocked = true;
	WeaponArsenal.Add(EWeaponSlot::Heavy, HeavyWeapon);

	FWeaponSlotData MeleeWeapon;
	MeleeWeapon.Slot = EWeaponSlot::Melee;
	MeleeWeapon.WeaponName = TEXT("Martial Arts / Brawling");
	MeleeWeapon.CurrentAmmo = 999;
	MeleeWeapon.MaxClipSize = 999;
	MeleeWeapon.ReserveAmmo = 999;
	MeleeWeapon.BaseDamage = 45.0f;
	MeleeWeapon.RoundsPerMinute = 180.0f;
	MeleeWeapon.bIsUnlocked = true;
	WeaponArsenal.Add(EWeaponSlot::Melee, MeleeWeapon);
}

void UCombatInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	SwitchToSlot(EWeaponSlot::Primary);
}

void UCombatInventoryComponent::SwitchToSlot(EWeaponSlot NewSlot)
{
	if (bIsReloading)
	{
		// Cancel ongoing reload on quick weapon switch
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(ReloadTimerHandle);
		}
		bIsReloading = false;
	}

	if (WeaponArsenal.Contains(NewSlot) && WeaponArsenal[NewSlot].bIsUnlocked)
	{
		CurrentSlot = NewSlot;
		const FWeaponSlotData& Data = WeaponArsenal[CurrentSlot];
		OnWeaponSwitched.Broadcast(CurrentSlot, Data);
		OnAmmoChanged.Broadcast(Data.CurrentAmmo, Data.ReserveAmmo);
	}
}

void UCombatInventoryComponent::NextWeapon()
{
	uint8 CurrentIndex = static_cast<uint8>(CurrentSlot);
	uint8 NextIndex = (CurrentIndex + 1) % 4;
	SwitchToSlot(static_cast<EWeaponSlot>(NextIndex));
}

void UCombatInventoryComponent::PreviousWeapon()
{
	uint8 CurrentIndex = static_cast<uint8>(CurrentSlot);
	uint8 PrevIndex = (CurrentIndex == 0) ? 3 : (CurrentIndex - 1);
	SwitchToSlot(static_cast<EWeaponSlot>(PrevIndex));
}

bool UCombatInventoryComponent::ConsumeAmmo(int32 Amount)
{
	if (bIsReloading) return false;

	if (FWeaponSlotData* Data = WeaponArsenal.Find(CurrentSlot))
	{
		if (CurrentSlot == EWeaponSlot::Melee)
		{
			return true; // Melee is infinite
		}

		if (Data->CurrentAmmo >= Amount)
		{
			Data->CurrentAmmo -= Amount;
			OnAmmoChanged.Broadcast(Data->CurrentAmmo, Data->ReserveAmmo);

			if (Data->CurrentAmmo <= 0 && Data->ReserveAmmo > 0)
			{
				Reload();
			}
			return true;
		}
		else if (Data->ReserveAmmo > 0)
		{
			Reload();
		}
	}
	return false;
}

void UCombatInventoryComponent::Reload()
{
	if (bIsReloading || CurrentSlot == EWeaponSlot::Melee) return;

	if (FWeaponSlotData* Data = WeaponArsenal.Find(CurrentSlot))
	{
		if (Data->CurrentAmmo >= Data->MaxClipSize || Data->ReserveAmmo <= 0)
		{
			return; // Clip full or no reserve
		}

		bIsReloading = true;
		OnReloadStarted.Broadcast(ReloadTimeSeconds);

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ReloadTimerHandle, this, &UCombatInventoryComponent::FinishReload, ReloadTimeSeconds, false);
		}
	}
}

void UCombatInventoryComponent::FinishReload()
{
	bIsReloading = false;

	if (FWeaponSlotData* Data = WeaponArsenal.Find(CurrentSlot))
	{
		int32 Needed = Data->MaxClipSize - Data->CurrentAmmo;
		int32 ToTransfer = FMath::Min(Needed, Data->ReserveAmmo);

		Data->CurrentAmmo += ToTransfer;
		Data->ReserveAmmo -= ToTransfer;

		OnAmmoChanged.Broadcast(Data->CurrentAmmo, Data->ReserveAmmo);
	}
}

void UCombatInventoryComponent::AddReserveAmmo(EWeaponSlot Slot, int32 Amount)
{
	if (FWeaponSlotData* Data = WeaponArsenal.Find(Slot))
	{
		Data->ReserveAmmo += Amount;
		if (Slot == CurrentSlot)
		{
			OnAmmoChanged.Broadcast(Data->CurrentAmmo, Data->ReserveAmmo);
		}
	}
}

const FWeaponSlotData& UCombatInventoryComponent::GetCurrentWeaponData() const
{
	static const FWeaponSlotData DefaultData;
	if (const FWeaponSlotData* Found = WeaponArsenal.Find(CurrentSlot))
	{
		return *Found;
	}
	return DefaultData;
}

bool UCombatInventoryComponent::CanFire() const
{
	if (bIsReloading) return false;
	if (const FWeaponSlotData* Data = WeaponArsenal.Find(CurrentSlot))
	{
		return Data->CurrentAmmo > 0 || CurrentSlot == EWeaponSlot::Melee;
	}
	return false;
}
