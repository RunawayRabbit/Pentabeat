// Fill out your copyright notice in the Description page of Project Settings.


#include "RCameraModifier_FocusTargetActor.h"

#include "GP4_Team7/GameModes/RGameModeBase.h"
#include "GP4_Team7/GameModes/RPlayerTargetingComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetMathLibrary.h"


URCameraModifier_FocusTargetActor::URCameraModifier_FocusTargetActor(
	const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer), PrevTarget(nullptr)
{
}

bool URCameraModifier_FocusTargetActor::ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
                                                            FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);

#if 1
	// Early-out if we have no ViewTarget (probably always the player in the context of our game)..
	if (!IsValid(ViewTarget)) return false;

	// ..Or if the GameMode isn't set correctly..
	ARGameModeBase* RGameMode = Cast<ARGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!RGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraMod:FocusTargetActor: GameMode is not RGameModeBase!"));
		return false;
	}
	// ..Or if the TargetingComponent is missing..
	URPlayerTargetingComponent* TargetingComponent = RGameMode->FindComponentByClass<URPlayerTargetingComponent>();
	if (!TargetingComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraMod:FocusTargetActor: GameMode doesn't have a URPlayerTargetingComponent!"));
		return false;
	}
	// ..Or if the player has no target.
	if (!TargetingComponent->IsTargeting())
	{
		UE_LOG(LogTemp, Display, TEXT("CameraMod:FocusTargetActor: Player has no target"));
		return false;
	}


	// Early-out checks are done. Moving on to the meat.
	AActor* CurrentTarget = TargetingComponent->GetCurrentTarget();

	if (CurrentTarget != PrevTarget)
	{
		UE_LOG(LogTemp, Display, TEXT("CameraMod:FocusTargetActor targetting a new actor"));
		//@TODO(ejtb): Do we need to do something special when we select a new target? Do we ever do that?


		// Update PrevTarget so we don't repeat this step next frame
		PrevTarget = CurrentTarget;
	}

	const FVector CameraLocation = CameraOwner->GetCameraLocation();
	const FVector TargetLocation = CurrentTarget->GetActorLocation();

	// "perfect" rotation, pointing directly at the boss.
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);

	// Current rotation of the camera
	const FRotator DeltaToTarget = (TargetRotation - OutViewRotation).GetNormalized();

	if (MaximumDeviationYaw <= 0.0f)
	{
		OutViewRotation.Yaw = TargetRotation.Yaw;
	}
	else
	{
		if (FMath::Abs(DeltaToTarget.Yaw) > MaximumDeviationYaw)
		{
			// We have strayed too far from the boss. Snap at the closest yaw angle
			OutViewRotation.Yaw = TargetRotation.Yaw - (MaximumDeviationYaw * FMath::Sign(DeltaToTarget.Yaw));
		}
		else
		{
			// Make a small adjustment to face the boss, based on  our delta.
			OutViewRotation.Yaw += SoftTrackingSpeed * DeltaTime * (DeltaToTarget.Yaw / MaximumDeviationYaw);
		}
	}

/*
	if (SnapRotationSpeed <= 0.0f)
	{
		// We define a SnapRotationSpeed of 0 to be instant snapping.
		OutViewRotation = TargetRotation;
	}
	else
	{
		const FRotator DeltaToTarget = TargetRotation - OutViewRotation;
		const FRotator DeltaToTargetNorm = DeltaToTarget.GetNormalized();

		UE_LOG(LogTemp, Warning, TEXT("DeltaToTargetNorm: P:%d  R:%r  Y:%y"), DeltaToTargetNorm.Pitch, DeltaToTargetNorm.Roll,
		       DeltaToTargetNorm.Yaw);

		if (FMath::Abs(DeltaToTarget.Yaw) > 30.0f)
		{
			if (DeltaToTargetNorm.IsNearlyZero())
			{
				// We're almost there, snap and call it done.
				OutViewRotation = TargetRotation;
			}
			else
			{
				// Rotate towards TargetRotation at Snapped Speed
				OutDeltaRot += DeltaToTargetNorm * SnapRotationSpeed * DeltaTime;
			}
		}
	}*/


	// Use player input to point the camera
	/*
			const FVector2D TargetSelectionInput = TargetingActor->GetCurrentTargetSelectionInput();

			FRotator DeltaRot;
			DeltaRot.Yaw = TargetSelectionInput.X * RotationSpeed * DeltaTime;
			DeltaRot.Pitch = -TargetSelectionInput.Y * RotationSpeed * DeltaTime;
			DeltaRot.Roll = 0.0f;

			OutDeltaRot += DeltaRot;
			*/


	//UE_LOG(LogTemp, Display, TEXT("CameraMod:FocusTargetActor: DeltaRot P:%f Y:%f R:%f"), OutDeltaRot.Pitch, OutDeltaRot.Yaw, OutDeltaRot.Roll);
#endif
	// Return value of true prevents downstream modifiers from being applied.
	return true;
}
