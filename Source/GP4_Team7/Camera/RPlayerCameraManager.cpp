// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerCameraManager.h"

ARPlayerCameraManager::ARPlayerCameraManager(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
	LineOfSightProbeWidth = 20.0f;
	LineOfSightProbeChannel = ECC_Camera;
}

void ARPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);
}
