// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"

#include "RMovementComponent.generated.h"


class ARPlayerPawn;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API URMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

	//Methods:
public:
	URMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

	void SetHorizontalAxis(float axis);
	void SetVerticalAxis(float axis);

	void SetGamepadHorizontalAxis(float axis);
	void SetGamepadVerticalAxis(float axis);
	
	void RotateToInput(FVector InputVector);

	bool HasReceivedInput() const;

	UFUNCTION(BlueprintCallable)
	float GetSpeedPercent();
protected:
	void Accelerate(float DeltaTime, float InputMagnitude);
	void SetSpeed(float InputMagnitude);
	void Decelerate(float DeltaTime);

	//Variables:
public:
	UPROPERTY(EditAnywhere)
	float MaxMovementSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float Acceleration = 2000.0f;

	UPROPERTY(EditAnywhere)
	float Deceleration = 2000.0f;

	UPROPERTY()
	float CurrentSpeed = 0.0f;

	UPROPERTY()
	bool UsingKeyboardThisFrame = false;
protected:
	UPROPERTY()
	float HorizontalAxis = 0.0f;

	UPROPERTY()
	float VerticalAxis = 0.0f;

	UPROPERTY()
	float GamepadHorizontalAxis = 0.0f;

	UPROPERTY()
	float GamepadVerticalAxis = 0.0f;

	UPROPERTY()
	ARPlayerPawn* Owner;
};
