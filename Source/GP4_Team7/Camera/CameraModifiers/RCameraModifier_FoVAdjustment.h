// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GP4_Team7/Camera/RCameraModifier.h"

#include "RCameraModifier_FoVAdjustment.generated.h"

/**
 *
 */
UCLASS()
class GP4_TEAM7_API URCameraModifier_FoVAdjustment : public URCameraModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DesiredVerticalFoV = 100.0f;

	public:
	URCameraModifier_FoVAdjustment(const FObjectInitializer& = FObjectInitializer::Get());

	virtual bool ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;
};
