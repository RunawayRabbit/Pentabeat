// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPlayerCameraManager.generated.h"


UCLASS(NotPlaceable, Transient, BlueprintType, Blueprintable)
class GP4_TEAM7_API ARPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ARPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Radius of the probe for checking line of sight between camera and the target */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float LineOfSightProbeWidth;

	/** Collision channel to use when checking line of sight */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	TEnumAsByte<ECollisionChannel> LineOfSightProbeChannel;

protected:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
