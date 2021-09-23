// Fill out your copyright notice in the Description page of Project Settings.

#include "RCameraSpringArmComponent.h"

#include "GP4_Team7/GameModes/RGameModeBase.h"
#include "GP4_Team7/GameModes/RPlayerTargetingComponent.h"
#include "Kismet/GameplayStatics.h"

URCameraSpringArmComponent::URCameraSpringArmComponent(
	const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Enable camera controls.
	bUsePawnControlRotation = true;

	// Set reasonable default values for camera distance and perspective.
	SocketOffset = FVector(0.0f, 0.0f, 40.0f);

	// Enable camera lag.
	bEnableCameraLag = true;
	CameraLagSpeed = 5.0f;
}


void URCameraSpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag,
                                                          float DeltaTime)
{

	// @NOTE: One big optimization is that all of this code assumes that the center of the play area, and the
	// pivot around which the boss rotates, is the ORIGIN of the coordinate system in WORLD space. That's a
	// gigantic assumption. NOTHING CAN POSSIBLY GO WRONG QUESTION MARK

	if (!TargetingComponent || !TargetingComponent->IsTargeting())
	{
		return;
	}

	const auto PlayerLoc = GetOwner()->GetActorLocation();
	const auto BossLoc = TargetingComponent->GetCurrentTarget()->GetActorLocation();

	const FVector ForwardVector = BossLoc.GetSafeNormal();
	const FVector RightVector = FVector::CrossProduct(FVector::UpVector, ForwardVector);

	const FMatrix Matrix(ForwardVector, RightVector, FVector::UpVector, -BossLoc);

	//DrawDebugCoordinateSystem(GetWorld(), FVector::UpVector* 30.0f, Matrix.Rotator(), 300.0f);

	const float BossRange = FMath::Clamp(
        FVector::DotProduct(BossLoc + BossLoc, PlayerLoc + BossLoc) /
        FVector::DotProduct(BossLoc + BossLoc, BossLoc + BossLoc),
        0.0f, 1.0f);

	const FVector LeftyRighty = RightVector * BossLoc.Size();

	const float LeftRightPanning = FMath::Clamp(
        FVector::DotProduct(LeftyRighty + LeftyRighty, PlayerLoc + LeftyRighty) /
        FVector::DotProduct(LeftyRighty + LeftyRighty, LeftyRighty + LeftyRighty),
        0.0f, 1.0f);


	FVector Candidate(
		FMath::Lerp(CameraMaxDistance, CameraMinDistance, BossRange), // back/front
		FMath::Lerp(-MaxCameraSideways, MaxCameraSideways, LeftRightPanning), // left/right
		FMath::Lerp(CameraMaxHeight, CameraMinHeight, BossRange) // up/down
	);

	Candidate = Matrix.TransformVector(Candidate);


	/*

	FVector2D PlayerScreenLocation;
	const auto PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ProjectWorldLocationToScreen(PlayerLoc, PlayerScreenLocation);

	PlayerScreenLocation.X /= ViewportWidth;
	PlayerScreenLocation.Y /= ViewportHeight;
	//PlayerScreenLocation = (PlayerScreenLocation * 2.0f) - FVector2D::UnitVector;

	float HeightAdjustX = 0.0f;
	if(FMath::Abs(PlayerScreenLocation.Y) > PlayerFramingZoom)
	{
		HeightAdjustX = FMath::GetMappedRangeValueClamped({PlayerFramingZoom, 0.9f},{0.0f, CameraMaxHeight}, FMath::Abs(PlayerScreenLocation.Y) );
		Candidate.X -= 0.5f*HeightAdjustX * FMath::Sign(PlayerScreenLocation.Y);
	}

	float HeightAdjustY = 0.0f;
	if(FMath::Abs(PlayerScreenLocation.X) > PlayerFramingZoom)
	{
		HeightAdjustY = FMath::GetMappedRangeValueClamped({PlayerFramingZoom, 0.9f},{0.0f, CameraMaxHeight}, FMath::Abs(PlayerScreenLocation.X) );
		Candidate.Y -= 0.5f*HeightAdjustY * FMath::Sign(PlayerScreenLocation.X);
	}

	*/


	if (ShouldTrackJump) Candidate.Z += (PlayerLoc.Z - BossLoc.Z) * JumpTrackingWeight;

	TargetOffset = Candidate - PlayerLoc;
	Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);
}

void URCameraSpringArmComponent::BeginPlay()
{
	ARGameModeBase* RGameMode = Cast<ARGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!RGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraMod:FocusTargetActor: GameMode is not RGameModeBase!"));
		return;
	}

	TargetingComponent = RGameMode->FindComponentByClass<URPlayerTargetingComponent>();
}
