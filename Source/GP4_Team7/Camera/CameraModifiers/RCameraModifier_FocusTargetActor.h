// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GP4_Team7/Camera/RCameraModifier.h"
#include "RCameraModifier_FocusTargetActor.generated.h"

/**
 *  Points the camera at the target actor.
 */
UCLASS(BlueprintType, Blueprintable)
class GP4_TEAM7_API URCameraModifier_FocusTargetActor : public URCameraModifier
{
	GENERATED_BODY()

public:
	URCameraModifier_FocusTargetActor(const FObjectInitializer& = FObjectInitializer::Get());

	virtual bool ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;

private:

	/** How far away the camera should allow itself to be angled relative to the boss. 0 = always pointing directly at the boss. */
	UPROPERTY(Category = Custom, EditDefaultsOnly, meta = (AllowPrivateAcces = "true"))
	float MaximumDeviationYaw;

	/** Speed at which the camera "glides" towards the boss in degrees/second */
	UPROPERTY(Category = Custom, EditDefaultsOnly, meta = (AllowPrivateAcces = "true"))
	float SoftTrackingSpeed;

	/** Time to wait after player input before we begin soft tracking. NOT YET IMPLEMENTED */
	UPROPERTY(Category = Custom, EditDefaultsOnly, meta = (AllowPrivateAcces = "true"))
	float SoftTrackingDelay;

	/** Actor that we were following on the previous frame */
	AActor* PrevTarget;
};
