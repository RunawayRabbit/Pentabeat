// Fill out your copyright notice in the Description page of Project Settings.


#include "RCameraModifier.h"

#include "RCameraSpringArmComponent.h"

URCameraModifier::URCameraModifier(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer),
	  CurrentModifiers(FRCameraInfo()),
	  TargetModifiers(FRCameraInfo())
{
}

bool URCameraModifier::ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);

	// early-out if we can't get an APlayerController
	if(!IsValid(ViewTarget)) return false;

	APawn* Pawn = Cast<APawn>(ViewTarget);
	if(!IsValid(Pawn)) return false;

	APlayerController* PlayerController = Cast<APlayerController>(Pawn->Controller);
	if(!IsValid(PlayerController)) return false;


	if (!PlayerController->RotationInput.IsNearlyZero(THRESH_QUAT_NORMALIZED))
	{
		// Player is providing input, reset the cooldown!
		CooldownRemaining = CooldownAfterInput;
	}
	else
	{
		CooldownRemaining -= DeltaTime;
	}

	return false;

}

bool URCameraModifier::PlayerHasRecentlyChangedCamera() const
{
	return CooldownRemaining > 0;
}

FRCameraInfo URCameraModifier::GetCurrentModifiers() const
{
	return CurrentModifiers;
}

FRCameraInfo URCameraModifier::GetTargetModifiers() const
{
	return TargetModifiers;
}

void URCameraModifier::ApplyCameraTransition(const FRCameraInfo CamInfo, const float TransitionTime, struct FMinimalViewInfo& InOutPOV,
                                             const float DeltaTime)
{
	if(CamInfo != TargetModifiers)
	{
		// We have been given a new target to move towards.
		CurrentModifiers = TargetModifiers; //@TODO(ejtb): Not sure about this line, should we snap like this?
		TargetModifiers = CamInfo;

		if(RemainingTransitionTime > 0.0f)
		{
			// We were interrupted during a transition!
			// @TODO(ejtb): Source material has us play the transition backwards in this case. Is this a good solution for us?
			RemainingTransitionTime = TotalTransitionTime - RemainingTransitionTime;
		}
	}

	FRCameraInfo ModifiersToApply;
	float ApplicationFactor = 0.0f; // How far through the transition are we?
	if(CurrentModifiers == TargetModifiers)
	{
		// We are right where we want to be.
		ModifiersToApply = CurrentModifiers;
		ApplicationFactor = 1.0f;
	}
	else // CurrentModifiers != TargetModifiers
	{
		//@TODO(ejtb) Refactor all'a dis shit

		// We aren't where we want to be. Let's work this shit out.

		if(TargetModifiers.IsNotZero())
		{
			// We have a target.
			ModifiersToApply = TargetModifiers;

			if(RemainingTransitionTime <= 0.0f)
			{
				// We have a target, but we aren't transitioning. This is our cue
				// to start a new transition
				TotalTransitionTime = TransitionTime;
				RemainingTransitionTime = TotalTransitionTime;
			}
			else // RemainingTransitionTime > 0.0f
			{
				// We have a target and are currently transitioning,
				// so let's continue!
				RemainingTransitionTime -= DeltaTime;

				if(RemainingTransitionTime <= 0.0f)
				{
					// Transition is ending
					CurrentModifiers = TargetModifiers;
					ApplicationFactor = 1.0f;
				}
				else
				{
					//We are mid-transition
					ApplicationFactor = 1.0f - (RemainingTransitionTime / TotalTransitionTime);
				}
			}
		}
		else //TargetModifiers.IsZero()
		{
			// We are blending towards a null target. This means "blend out this modifier".
			ModifiersToApply = CurrentModifiers;

			if(RemainingTransitionTime <= 0.0f)
			{
				// Start transitioning from 1.0 towards 0.0!
				RemainingTransitionTime = TotalTransitionTime;
				ApplicationFactor = 1.0f;
			}
			else // RemainingTransitionTime > 0.0f
			{
				// Gradually transition towards zero over TotalTransitionTime
				ApplicationFactor = RemainingTransitionTime / TotalTransitionTime;
			}
		}
	}

	// Apply the modifiers
	ApplyCameraInfo(ModifiersToApply, ApplicationFactor, InOutPOV);
}

void URCameraModifier::ApplyCameraInfo(const FRCameraInfo& CamInfo, const float Factor, struct FMinimalViewInfo& InOutPOV) const
{
	AActor* ViewTarget = GetViewTarget();
	if (!IsValid(ViewTarget)) return;

	// Apply the FoV, after scaling it.
	//@TODO(ejtb): Does this make any sense at all? At 0, our FoV becomes 0. Shouldn't it become some default value?
	InOutPOV.FOV = CamInfo.FoV * Factor;

	// Get the SpringArmComponent
	auto CamSpringArmComponent = ViewTarget->FindComponentByClass<URCameraSpringArmComponent>();
	// Set the sprint arm's length
	CamSpringArmComponent->TargetArmLength += CamInfo.SpringArmLength * Factor;

	//@TODO(ejtb): DoF stuff
}
