// Yeet <3

#pragma once

#include "CoreMinimal.h"
#include "RDA_TutorialTextSettings.generated.h"

UCLASS(BlueprintType, Blueprintable)
class GP4_TEAM7_API URDA_TutorialTextSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableTextFadeOut = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DisplayDurationSecondsBeforeFadeOut = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeOutAnimationDurationSeconds = 1.f;
};
