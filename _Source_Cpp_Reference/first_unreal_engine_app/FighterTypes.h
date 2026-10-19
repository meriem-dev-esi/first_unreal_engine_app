#pragma once

#include "CoreMinimal.h"
#include "FighterTypes.generated.h"

UENUM(BlueprintType)
enum class EFighterCombatState : uint8
{
	Idle            UMETA(DisplayName = "Idle"),
	Attacking       UMETA(DisplayName = "Attacking"),
	Blocking        UMETA(DisplayName = "Blocking"),
	Dodging         UMETA(DisplayName = "Dodging"),
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
	SuperSlam       UMETA(DisplayName = "Super Ground Slam"),
	ChargedShot     UMETA(DisplayName = "Charged Projectile")
};

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart   UMETA(DisplayName = "Waiting To Start"),
	WaveInProgress   UMETA(DisplayName = "Wave In Progress"),
	Intermission     UMETA(DisplayName = "Intermission"),
	BossWave         UMETA(DisplayName = "Boss Wave"),
	Victory          UMETA(DisplayName = "Victory"),
	GameOver         UMETA(DisplayName = "Game Over")
};

UENUM(BlueprintType)
enum class EPowerupType : uint8
{
	HealthPack      UMETA(DisplayName = "Health Pack (+50 HP)"),
	ShieldBoost     UMETA(DisplayName = "Armor / Shield (+50 Armor)"),
	DamageBoost     UMETA(DisplayName = "Double Damage (15s)"),
	SpeedBoost      UMETA(DisplayName = "Speed Surge (+50% Speed)"),
	InstantSuper    UMETA(DisplayName = "Instant Ultimate Charge")
};

UENUM(BlueprintType)
enum class ECombatRank : uint8
{
	Rank_D          UMETA(DisplayName = "Rank D (Beginner)"),
	Rank_C          UMETA(DisplayName = "Rank C (Fighter)"),
	Rank_B          UMETA(DisplayName = "Rank B (Warrior)"),
	Rank_A          UMETA(DisplayName = "Rank A (Champion)"),
	Rank_S          UMETA(DisplayName = "Rank S (Legendary)"),
	Rank_SSS        UMETA(DisplayName = "Rank SSS (Godlike)")
};

USTRUCT(BlueprintType)
struct FWaveConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 WaveNumber = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 MeleeEnemies = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 ShooterEnemies = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	bool bIsBossWave = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float EnemyHealthScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float EnemySpeedScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 WaveClearBonusScore = 500;
};

USTRUCT(BlueprintType)
struct FCombatScoreData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 TotalScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 TotalKills = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 CurrentCombo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 MaxComboStreak = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	float ScoreMultiplier = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	ECombatRank CurrentRank = ECombatRank::Rank_D;
};
