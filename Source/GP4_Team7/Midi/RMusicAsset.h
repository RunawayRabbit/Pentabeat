// Yeet <3

#pragma once

#include "CoreMinimal.h"

#include "RMusicAsset.generated.h"

class UMidiAsset;
class USoundBase;

UCLASS(BlueprintType)
class GP4_TEAM7_API URMusicAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMidiAsset* Midi;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Music;
};
