// Fill out your copyright notice in the Description page of Project Settings.

#include "RMusicManager.h"

#include "Components/AudioComponent.h"


// Sets default values for this component's properties
URMusicManager::URMusicManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Music Player"));
}

void URMusicManager::BeginPlay()
{
	Super::BeginPlay();

	if (MusicAsset != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *MusicAsset->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid MusicAsset."));
	}


	this->OnStart.AddDynamic(this, &URMusicManager::OnStartPlaying);
	this->OnStop.AddDynamic(this, &URMusicManager::OnFinishPlaying);

	this->OnMidiEvent.AddDynamic(this, &URMusicManager::MidiEvent);

	this->OnTempoEvent.AddDynamic(this, &URMusicManager::OnTempoChange);
	this->OnMetronomeTick.AddDynamic(this, &URMusicManager::OnBeat);

	this->OnTextEvent.AddDynamic(this, &URMusicManager::OnTextReceived);

	LoadMusicAsset(MusicAsset);
}

void URMusicManager::PlayMusicAsset()
{
	if (MusicAsset == nullptr)
	{
		return;
	}

	Super::start(false, true);
	AudioComponent->Play();
}

void URMusicManager::StopMusicAsset()
{
	if (MusicAsset == nullptr)
	{
		return;
	}

	Super::stop();
	AudioComponent->Stop();
}

void URMusicManager::RestartMusicAsset()
{
	if (MusicAsset == nullptr)
	{
		return;
	}
	Super::reset();
	AudioComponent->Stop();
	Super::start(false, true);
	AudioComponent->Play();
}


void URMusicManager::OnTextReceived(EMidiTextTypeEnum Type,  const FString& Text,  int32 Time, const int TrackID)
{
	MarkerEvent.Broadcast(Text);
}

void URMusicManager::GoBackToMarker(const FString& Marker)
{
	//@NOTE(ejtb): Due to, uhm, "technical limitations", the marker that you jump to will still play.

	UE_LOG(LogTemp, Display, TEXT("Jumping to: %s"), *Marker);
	const float millisecondsElapsed = playFromMarker(Marker);

	AudioComponent->Play(millisecondsElapsed / 1000.0f);

}

void URMusicManager::LoadMusicAsset(URMusicAsset* NewMusicAsset)
{
	if (NewMusicAsset == nullptr ||
		NewMusicAsset->Music == nullptr ||
		NewMusicAsset->Midi == nullptr)
	{
		return;
	}

	MusicAsset = NewMusicAsset;
	Super::LoadAsset(NewMusicAsset->Midi);
	AudioComponent->SetSound(NewMusicAsset->Music);
}

void URMusicManager::OnStartPlaying(bool Beginning)
{
	FMusicEventInfo Info{};
	Info.Type = ERMusicEventType::MusicStarted;
	MusicEvent.Broadcast(Info);
}

void URMusicManager::OnFinishPlaying(bool Finished)
{
	FMusicEventInfo Info{};
	Info.Type = ERMusicEventType::MusicStopped;
	MusicEvent.Broadcast(Info);
}


void URMusicManager::MidiEvent(const FMidiEvent Event, int32 Time, const int TrackID)
{
	if (TrackID > MIDI_TRACK_COUNT)
	{
		UE_LOG(LogTemp, Error, TEXT("URTrackManager: I thought there couldn't be more than 16 tracks but I was WRONG?!?!?!"));
		return;
	}

	if (Event.Type == EMidiTypeEnum::MTE_CONTROLLER)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d| Data1: %d Data 2: %d"), Time, static_cast<int>(Event.Data1), static_cast<int>(Event.Data2));

		if (Event.Data1 == 7)
		{
			// Volume
			TrackVolume[TrackID] = Event.Data2 / 127.0f;
		}
		else if (Event.Data1 == 8)
		{
			// Balance
		}
		else if (Event.Data1 == 10)
		{
			// Panning
			TrackPanning[TrackID] = 1.0f - (Event.Data2 / 64.0f);
		}
	}

#if 1
	if (Event.Type == EMidiTypeEnum::MTE_NOTE)
	{
		/*
		*	Note On
		*	[Data1=Note, Data2=Velocity]
		*	Note: Velocity of 0 = same as Note Off
		*/
		FMusicEventInfo Info;

		Info.Type = ERMusicEventType::NotePlayed;
		//Info.Note = Event.Data1;
		Info.Volume = (Event.Data2 / 127.0f) * TrackVolume[TrackID];
		Info.Track = TrackID;
		Info.Panning = (Event.Data1-64.0f)/10.0f;

		MusicEvent.Broadcast(Info);
	}
#endif
}

void URMusicManager::OnExtendedEvent(const TArray<uint8>& Data, int32 Time, const int TrackID)
{
	UE_LOG(LogTemp, Display, TEXT("MIDI EXT EVENT (%d): %s"), TrackID, &Data);
}

void URMusicManager::OnBeat(int BeatNumber, int Measure, int32 Time)
{
	FMusicEventInfo Info{};

	Info.Type = ERMusicEventType::Beat;

	MusicEvent.Broadcast(Info);
	BeatEvent.Broadcast(BeatNumber, Measure, Time, CurrentBPM);
}

void URMusicManager::OnTempoChange(float BPM, int32 Time)
{
	FMusicEventInfo Info{};

	Info.Type = ERMusicEventType::TempoChanged;
	this->CurrentBPM = BPM;

	MusicEvent.Broadcast(Info);
}
