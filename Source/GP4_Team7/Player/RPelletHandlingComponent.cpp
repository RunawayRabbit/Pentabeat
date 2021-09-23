// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_Team7/Player/RPelletHandlingComponent.h"

#include "GameFramework/GameModeBase.h"
#include "GP4_Team7/Hostiles/RPellet.h"
#include "GP4_Team7/GameModes/RPlayerTargetingComponent.h"
#include "Kismet/GameplayStatics.h"

URPelletHandlingComponent::URPelletHandlingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URPelletHandlingComponent::PickupPellet(ARPellet* PickedUpPellet)
{
	if(CurrentPelletCount >= MaxPellets)
	{
		OnPelletAttemptPickupFull.Broadcast(CurrentPelletCount);
		return;
	}

	PickedUpPellet->Destroy();

	ChangePelletCount(CurrentPelletCount + 1);
}

bool URPelletHandlingComponent::AttemptPelletBlock()
{
	if(CurrentPelletCount <= 0)
		return false;

	OnPelletBlock.Broadcast(CurrentPelletCount);
	const int PelletsAfterBlocking = LoseAllPelletsWhenBlocking ? 0 : CurrentPelletCount-1;
	ChangePelletCount(PelletsAfterBlocking);
	return true;
}

void URPelletHandlingComponent::ShootPellets()
{
	if(CurrentPelletCount <= 0)
	{
		OnPelletAttemptShootEmpty.Broadcast(CurrentPelletCount);
		return;
	}

	float damagePerPellet = 0.0f;

	switch(CurrentPelletCount)
	{
	case 1:
		damagePerPellet = OnePelletDamage;
		break;
	case 2:
		damagePerPellet = TwoPelletDamage;
		break;
	case 3:
		damagePerPellet = ThreePelletDamage;
		break;
	default:
		break;
	}

	OnPelletShoot.Broadcast(CurrentPelletCount, TargetingComponent->PlayerTarget, damagePerPellet);
	const int PelletsAfterShooting = LoseAllPelletsWhenShooting ? 0 : CurrentPelletCount-1;
	ChangePelletCount(PelletsAfterShooting);
}


void URPelletHandlingComponent::BeginPlay()
{
	Super::BeginPlay();

	UActorComponent* actorComp = UGameplayStatics::GetGameMode(this)->GetComponentByClass(URPlayerTargetingComponent::StaticClass());
	URPlayerTargetingComponent* targetingComp = Cast<URPlayerTargetingComponent>(actorComp);
	if(targetingComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find RPlayerTargetingComponent on GameMode from URPelletHandlingComponent"))
	}
	else
	{
		TargetingComponent = targetingComp;
	}
}

void URPelletHandlingComponent::ChangePelletCount(int NewCount)
{
	CurrentPelletCount = NewCount;
	OnPelletCountChange.Broadcast(CurrentPelletCount);
}

