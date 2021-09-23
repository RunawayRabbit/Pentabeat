#pragma once

#include "CoreMinimal.h"

#include "RProjectileStructs.generated.h"


USTRUCT(BlueprintType)
struct FRHomingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHoming = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PullAcceleration = 20000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialSpeed = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 1000.f;
	
	UPROPERTY()
	USceneComponent* HomingTargetComponent;
};
