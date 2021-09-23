// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FMusicEventInfo.h"
#include "MidiComponent.h"
#include "RMusicAsset.h"

#include "RMusicManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMusicEvent, struct FMusicEventInfo, MusicEventInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMusicMarkerEvent, const FString&, Marker);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMusicBeatEvent, int, BeatNumber, int, Measure, int32, Time, int32, BPM);

class UMidiComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_TEAM7_API URMusicManager : public UMidiComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URMusicManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	UPROPERTY(BlueprintAssignable)
	FMusicEvent MusicEvent;

	UPROPERTY(BlueprintAssignable)
	FMusicMarkerEvent MarkerEvent;

	UPROPERTY(BlueprintAssignable)
	FMusicBeatEvent BeatEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URMusicAsset* MusicAsset;

	UFUNCTION(BlueprintCallable)
	void PlayMusicAsset();

	UFUNCTION(BlueprintCallable)
	void StopMusicAsset();

	UFUNCTION(BlueprintCallable)
	void RestartMusicAsset();

	UFUNCTION(BlueprintCallable)
    void GoBackToMarker(const FString& Marker);

	UFUNCTION(BlueprintCallable)
	void LoadMusicAsset(URMusicAsset* NewMusicAsset);

	UPROPERTY(BlueprintReadOnly)
	UAudioComponent* AudioComponent;

	UPROPERTY(BlueprintReadOnly)
	int CurrentBPM;

private:

	UFUNCTION()
	void OnStartPlaying(bool Beginning);
	UFUNCTION()
	void OnFinishPlaying(bool Finished);

	UFUNCTION()
	void MidiEvent(struct FMidiEvent Event, int32 Time, int TrackID);

	UFUNCTION()
	void OnBeat(int BeatNumber, int Measure, int32 Time);
	UFUNCTION()
	void OnTempoChange(float BPM, int32 Time);

	UFUNCTION()
	void OnTextReceived(EMidiTextTypeEnum Type, const FString& Text, int32 Time, int TrackID);

	//NYI (at least not properly)

	UFUNCTION()
	static void OnExtendedEvent(const TArray<uint8>& Data, int32 Time, int TrackID);

	static const int MIDI_TRACK_COUNT = 16;

	float TrackPanning[MIDI_TRACK_COUNT];
	float TrackVolume[MIDI_TRACK_COUNT];
};
