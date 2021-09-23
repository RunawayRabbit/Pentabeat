// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RBoss.h"
#include "RProjectileDefinition.generated.h"

UCLASS(BlueprintType)
class GP4_TEAM7_API URProjectileDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ERBossSpawnType> ProjectileType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ERUpdateMovement> MovementType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRProjectileSpawnData ProjectileSpawnData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRBossMovementData BossMovementData;
};
