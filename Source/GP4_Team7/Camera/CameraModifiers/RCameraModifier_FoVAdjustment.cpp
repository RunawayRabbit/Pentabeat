// Fill out your copyright notice in the Description page of Project Settings.


#include "RCameraModifier_FoVAdjustment.h"

#include "Kismet/GameplayStatics.h"


URCameraModifier_FoVAdjustment::URCameraModifier_FoVAdjustment(
	const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
}

bool URCameraModifier_FoVAdjustment::ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
                                                         FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);

	int ViewportWidth, ViewportHeight;
	GetWorld()->GetFirstPlayerController()->GetViewportSize(ViewportWidth, ViewportHeight);

	// MAAAATH. Sorry for not making this the most readable.
	const float HorizontalFoV = FMath::RadiansToDegrees(
		2.0f * FMath::Atan(
			FMath::Tan(
				FMath::DegreesToRadians(DesiredVerticalFoV) / 2.0f)
				* ViewportWidth / ViewportHeight));

	CameraOwner->SetFOV(HorizontalFoV);

	return false;
}
