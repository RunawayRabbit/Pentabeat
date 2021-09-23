#include "GP4_Team7/Player/RMovementComponent.h"

#include "DrawDebugHelpers.h"
#include "GP4_Team7/Player/RPlayerPawn.h"
#include "Kismet/KismetMathLibrary.h"

URMovementComponent::URMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ARPlayerPawn>(GetOwner());
}


void URMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Owner->bIsDead)
		return;

	FVector inputVector = FVector::ZeroVector;
	inputVector.Y = HorizontalAxis;
	inputVector.X = VerticalAxis;
	float inputVectorMagnitude = inputVector.Size2D();
	inputVector = inputVector.GetSafeNormal2D();
	
	FVector gamepadInputVector = FVector::ZeroVector;
	gamepadInputVector.Y = GamepadHorizontalAxis;
	gamepadInputVector.X = GamepadVerticalAxis;
	float gamepadInputVectorMagnitude = gamepadInputVector.Size2D();
	gamepadInputVector = gamepadInputVector.GetSafeNormal2D();

	UsingKeyboardThisFrame = inputVectorMagnitude > gamepadInputVectorMagnitude;

	if(!inputVector.IsNearlyZero()
		|| !gamepadInputVector.IsNearlyZero())
	{

		if(UsingKeyboardThisFrame)
		{
			Accelerate(DeltaTime, inputVectorMagnitude);
			RotateToInput(inputVector);
		}
		else
		{
			SetSpeed(gamepadInputVectorMagnitude);
			RotateToInput(gamepadInputVector);
		}
	}
	else
	{
		Decelerate(DeltaTime);
	}
}

void URMovementComponent::SetHorizontalAxis(float axis)
{
	HorizontalAxis = axis;
}


void URMovementComponent::SetVerticalAxis(float axis)
{
	VerticalAxis = axis;
}

void URMovementComponent::SetGamepadHorizontalAxis(float axis)
{
	GamepadHorizontalAxis = axis;
}

void URMovementComponent::SetGamepadVerticalAxis(float axis)
{
	GamepadVerticalAxis = axis;
}

void URMovementComponent::RotateToInput(FVector InputVector)
{
	const FRotator ControlRotation = Owner->GetControlRotation();
	const FRotator YawOnly(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector rotatedInput = YawOnly.RotateVector(InputVector);

	const FRotator targetYaw = UKismetMathLibrary::MakeRotFromX(rotatedInput);

	FRotator newRotation = Owner->SkeletonMeshPivot->GetComponentRotation();
	newRotation.Yaw = targetYaw.Yaw;

	Owner->SkeletonMeshPivot->SetWorldRotation(newRotation);

	// Owner->SetActorRotation(targetRotation);
}

bool URMovementComponent::HasReceivedInput() const
{
	return !(FMath::IsNearlyZero(VerticalAxis) &&
        FMath::IsNearlyZero(HorizontalAxis));
}

float URMovementComponent::GetSpeedPercent()
{
	return CurrentSpeed/MaxMovementSpeed;
}

void URMovementComponent::Accelerate(float DeltaTime, float InputMagnitude)
{
	float potentialAcceleration = Acceleration * DeltaTime;
	if(CurrentSpeed >= MaxMovementSpeed + potentialAcceleration)
	{
		CurrentSpeed = MaxMovementSpeed;
		return;
	}
	CurrentSpeed += potentialAcceleration;
}

void URMovementComponent::SetSpeed(float InputMagnitude)
{
	CurrentSpeed = InputMagnitude * MaxMovementSpeed;
}

void URMovementComponent::Decelerate(float DeltaTime)
{
	float potentialDeceleration = Deceleration * DeltaTime;
	if(CurrentSpeed <= potentialDeceleration)
	{
		CurrentSpeed = 0;
		return;
	}
	CurrentSpeed -= potentialDeceleration;
}
