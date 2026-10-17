#include "FighterGameModeBase.h"
#include "TimerManager.h"
#include "FighterCharacter.h"
#include "Kismet/GameplayStatics.h"

AFighterGameModeBase::AFighterGameModeBase()
{
	DefaultPawnClass = AFighterCharacter::StaticClass();
}

void AFighterGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	StartNewRound();
}

void AFighterGameModeBase::StartNewRound()
{
	RoundTimeRemaining = RoundDurationSeconds;
	OnRoundTimerChanged.Broadcast(RoundTimeRemaining);

	GetWorldTimerManager().ClearTimer(RoundTimerHandle);
	GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &AFighterGameModeBase::UpdateRoundTimer, 1.0f, true);
}

void AFighterGameModeBase::UpdateRoundTimer()
{
	RoundTimeRemaining--;
	OnRoundTimerChanged.Broadcast(RoundTimeRemaining);

	if (RoundTimeRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(RoundTimerHandle);

		// Time Out: Determine winner by highest remaining health
		TArray<AActor*> Fighters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFighterCharacter::StaticClass(), Fighters);

		AActor* Winner = nullptr;
		float HighestHealthPct = -1.0f;

		for (AActor* FighterActor : Fighters)
		{
			if (AFighterCharacter* Fighter = Cast<AFighterCharacter>(FighterActor))
			{
				if (Fighter->GetHealthPercent() > HighestHealthPct)
				{
					HighestHealthPct = Fighter->GetHealthPercent();
					Winner = Fighter;
				}
			}
		}

		EndRound(Winner);
	}
}

void AFighterGameModeBase::EndRound(AActor* Winner)
{
	GetWorldTimerManager().ClearTimer(RoundTimerHandle);
	OnRoundEnded.Broadcast(CurrentRound, Winner);

	CurrentRound++;
}
