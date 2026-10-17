#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FighterGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundTimerChangedSignature, int32, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoundEndedSignature, int32, RoundNumber, AActor*, Winner);

UCLASS()
class FIRST_UNREAL_ENGINE_APP_API AFighterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFighterGameModeBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void StartNewRound();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void EndRound(AActor* Winner);

	UFUNCTION(BlueprintPure, Category = "GameMode")
	int32 GetCurrentRound() const { return CurrentRound; }

	UFUNCTION(BlueprintPure, Category = "GameMode")
	int32 GetRoundTimeRemaining() const { return RoundTimeRemaining; }

public:
	UPROPERTY(BlueprintAssignable, Category = "GameMode|Events")
	FOnRoundTimerChangedSignature OnRoundTimerChanged;

	UPROPERTY(BlueprintAssignable, Category = "GameMode|Events")
	FOnRoundEndedSignature OnRoundEnded;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRules")
	int32 RoundDurationSeconds = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameRules")
	int32 RoundsToWin = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameRules")
	int32 CurrentRound = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameRules")
	int32 RoundTimeRemaining = 90;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameRules")
	int32 PlayerWins = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameRules")
	int32 OpponentWins = 0;

	FTimerHandle RoundTimerHandle;

	void UpdateRoundTimer();
};
