// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_Team7/GameModes/RGameModeBase.h"

#include "Kismet/GameplayStatics.h"

ARGameModeBase::ARGameModeBase()
{
	for (auto SoundWave : PreloadSounds)
	{
		// SoundWave->CachePlatformData(false);
		SoundWave->OverrideLoadingBehavior(ESoundWaveLoadingBehavior::RetainOnLoad);
		UGameplayStatics::PrimeSound(SoundWave);
	}
}
