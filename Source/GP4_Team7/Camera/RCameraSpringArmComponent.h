// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"

#include "RCameraSpringArmComponent.generated.h"


class URPlayerTargetingComponent;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_TEAM7_API URCameraSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	URCameraSpringArmComponent(const FObjectInitializer& ObjectInitializer =  FObjectInitializer::Get());

	/** The closest that the camera can be to the floor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraMinHeight = 150.0f;

	/** The furthest that the camera can be from the floor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraMaxHeight = 500.0f;

	/** Affects how the camera zooms away from the player as they move off-screen. Lower numbers is smoother. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerFramingZoom = 0.45f;

	/** Should the camera move upwards when the player jumps? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldTrackJump = true;

	/** How much should the camera move when the player jumps? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpTrackingWeight = 0.5f;

	/**  position of the camera, relative to the arena's center, when the player is furthest from the boss */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraMaxDistance = -2000.0f;

	/**  position of the camera, relative to the arena's center, when the player is closest to the boss */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraMinDistance = 1800.0f;

	/** Maximum distance the camera will travel left or right relative to the frame */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCameraSideways = 800.0f;
	protected:

	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;

	protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	URPlayerTargetingComponent* TargetingComponent;

};
