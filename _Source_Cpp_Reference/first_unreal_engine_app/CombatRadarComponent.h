#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FighterTypes.h"
#include "CombatRadarComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadarScanCompletedSignature, const TArray<FRadarPingData>&, DetectedTargets);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class FIRST_UNREAL_ENGINE_APP_API UCombatRadarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatRadarComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Combat|Radar")
	void PerformRadarScan();

	UFUNCTION(BlueprintPure, Category = "Combat|Radar")
	const TArray<FRadarPingData>& GetLastScanResults() const { return CachedPings; }

	UFUNCTION(BlueprintPure, Category = "Combat|Radar")
	int32 GetHostileCount() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Combat|Radar|Events")
	FOnRadarScanCompletedSignature OnRadarScanCompleted;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Radar")
	float RadarScanRadius = 3500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Radar")
	float ScanIntervalSeconds = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Radar")
	TArray<FRadarPingData> CachedPings;

	FTimerHandle PeriodicScanTimer;
};
