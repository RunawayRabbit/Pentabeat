// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RCameraInfo.h"

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"

#include "RCameraModifier.generated.h"

UCLASS(BlueprintType, Blueprintable)
class GP4_TEAM7_API URCameraModifier : public UCameraModifier
{
	GENERATED_BODY()

public:
	URCameraModifier(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;


	//
	//	Cooldown Stuff
	//

public:
	/** Amount of time between the player adjusting the camera and the automatic adjustment starting again*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CooldownAfterInput;
protected:
	/** Used to make sure we don't immediately overwrite player-made adjustments with automatic ones */
	bool PlayerHasRecentlyChangedCamera() const;
private:
	/** Time remaining before automated camera movement resumes */
	float CooldownRemaining;


	//
	//	CameraInfo Stuff
	//

public:
	/** Gets Camera Info for the camera's current state */
	FRCameraInfo GetCurrentModifiers() const;
	/** Gets Camera Info for that we are transitioning towards */
	FRCameraInfo GetTargetModifiers() const;
private:
	FRCameraInfo CurrentModifiers;
	FRCameraInfo TargetModifiers;


	//
	// Transition Stuff
	//

protected:
	/**
	 * Applies a modifier to the camera, starting a transition if required.
	 *
	 * @param CamInfo The properties we should apply. Will become the new TargetModifier.
	 * @param TransitionTime Time in Seconds that the transition should take.
	 * @param InOutPOV Camera point of view being modified.
	 * @param DeltaTime Time in seconds since the last application of this modifier.
	 */
	void ApplyCameraTransition(const FRCameraInfo CamInfo, const float TransitionTime, struct FMinimalViewInfo& InOutPOV,
	                           const float DeltaTime);

private:
	void ApplyCameraInfo(const FRCameraInfo& CamInfo, const float Factor, struct FMinimalViewInfo& InOutPOV) const;

	float TotalTransitionTime;
	float RemainingTransitionTime;
};
