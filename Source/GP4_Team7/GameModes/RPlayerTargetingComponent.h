// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPlayerTargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API URPlayerTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

	//Methods:
public:
	URPlayerTargetingComponent();

	UFUNCTION(BlueprintPure)
	/** Returns the current target of the player. (Mostly the boss.) */
	AActor* GetCurrentTarget() const;

	/** Is the player currently targeting the boss? (Mostly used for camera purposes) */
	UFUNCTION(BlueprintPure)
	bool IsTargeting() const;

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* NewTarget);
	
	//Variables:
public:
	UPROPERTY()
	AActor* PlayerTarget = nullptr;
};
