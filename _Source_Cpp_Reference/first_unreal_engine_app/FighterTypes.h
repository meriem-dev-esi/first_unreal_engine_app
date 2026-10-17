#pragma once

#include "CoreMinimal.h"
#include "FighterTypes.generated.h"

UENUM(BlueprintType)
enum class EFighterCombatState : uint8
{
	Idle            UMETA(DisplayName = "Idle"),
	Attacking       UMETA(DisplayName = "Attacking"),
	Blocking        UMETA(DisplayName = "Blocking"),
	HitStun         UMETA(DisplayName = "Hit Stun"),
	KnockedDown     UMETA(DisplayName = "Knocked Down"),
	Dead            UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None            UMETA(DisplayName = "None"),
	Light           UMETA(DisplayName = "Light Attack"),
	Heavy           UMETA(DisplayName = "Heavy Attack"),
	Special         UMETA(DisplayName = "Special Attack")
};
