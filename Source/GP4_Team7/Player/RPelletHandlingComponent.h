// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPelletHandlingComponent.generated.h"

class ARPellet;
class URPlayerTargetingComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPelletDelegate, int, PelletCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPelletShootDelegate, int, PelletCount, AActor*, Target, float, Damage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API URPelletHandlingComponent : public UActorComponent
{
	GENERATED_BODY()

	//Methods:
public:
	URPelletHandlingComponent();

	UFUNCTION(BlueprintCallable)
	void PickupPellet(ARPellet* PickedUpPellet);

	bool AttemptPelletBlock(); // Yes, funky name but it's like blocking the damage with the pellets, otherwise it returns false
	void ShootPellets();

protected:
	virtual void BeginPlay() override;
	void ChangePelletCount(int NewCount);

	//Variables:
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxPellets = 10.0f;

	UPROPERTY(BlueprintAssignable)
	FPelletDelegate OnPelletBlock;

	UPROPERTY(BlueprintAssignable)
	FPelletShootDelegate OnPelletShoot;

	UPROPERTY(BlueprintAssignable)
	FPelletDelegate OnPelletCountChange;

	UPROPERTY(BlueprintAssignable)
	FPelletDelegate OnPelletAttemptPickupFull;

	UPROPERTY(BlueprintAssignable)
	FPelletDelegate OnPelletAttemptShootEmpty;

	UPROPERTY(EditAnywhere)
	float OnePelletDamage = 1.0f;

	UPROPERTY(EditAnywhere)
	float TwoPelletDamage = 3.0f;

	UPROPERTY(EditAnywhere)
	float ThreePelletDamage = 10.0f;

	UPROPERTY(EditAnywhere)
	bool LoseAllPelletsWhenShooting = true;

	UPROPERTY(EditAnywhere)
	bool LoseAllPelletsWhenBlocking = true;

protected:
	UPROPERTY(BlueprintReadOnly)
	int CurrentPelletCount = 0;

	UPROPERTY()
	URPlayerTargetingComponent* TargetingComponent;
};
