// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FMusicEventInfo.generated.h"


enum class Track1Instrument : uint8
{
	KickDrum = 1,
};

enum class Track2Instrument : uint8
{
	KickDrum = 1,
};


UENUM(BlueprintType)
enum class ERMusicEventType : uint8
{
	Error,
    MusicStarted,
    MusicStopped,
    NotePlayed,
    Beat,
    TempoChanged,
};

USTRUCT(BlueprintType)
struct FMusicEventInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Event Info")
	ERMusicEventType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Event Info")
	/** Should be from 0-15 */
	uint8 Track;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Event Info")
	/** Left = 0.0f, Right =  1.0f */
	float Panning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music Event Info")
	/** Log scale */
	float Volume;

	// uint8 Note;
};
