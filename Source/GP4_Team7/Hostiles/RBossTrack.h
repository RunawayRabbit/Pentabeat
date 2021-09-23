#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TrackTypes.h"

#include "RBossTrack.generated.h"

class UCurveVector;

class USplineComponent;
UCLASS(Blueprintable, BlueprintType)
class GP4_TEAM7_API ARBossTrack : public AActor // it's called BossTrack, but it's actually a general track / spline
{
	GENERATED_BODY()

public:
	ARBossTrack();

	void GenerateCircularTrack(const URDA_CircularTrackGenerationSettings* CircularTrackSettings);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineComponent* SplineComponent;
protected:
	
	// todo: May remove this option, not used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "Point Settings"))
	TArray<FRSplinePointData> SplinePointDatas;
	
private:	
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
