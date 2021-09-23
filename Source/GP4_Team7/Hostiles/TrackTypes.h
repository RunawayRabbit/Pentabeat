#pragma once

#include "TrackTypes.generated.h"

UENUM()
enum ERTrackType
{
	CustomBP UMETA(DisplayName = "Custom Track"),
	CircularGenerated UMETA(DisplayName = "Circular Generated Track"),
};

UCLASS()
class GP4_TEAM7_API URDA_CircularTrackGenerationSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Generation Settings: Circular Generated")
	float Radius = 1000.f;
};

USTRUCT(BlueprintType)
struct FRSplinePointData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDoSomething;

	friend bool operator==(const FRSplinePointData& Left, const FRSplinePointData& Right);
};
