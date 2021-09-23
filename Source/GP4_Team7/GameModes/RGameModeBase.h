// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class GP4_TEAM7_API ARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARGameModeBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<USoundWave*> PreloadSounds;
};
