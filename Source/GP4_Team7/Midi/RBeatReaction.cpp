// Yeet <3


#include "RBeatReaction.h"

#include "RMusicManager.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

URBeatReaction::URBeatReaction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void URBeatReaction::BeginPlay()
{
	Super::BeginPlay();

	const auto GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (!GameMode) return;

	auto MusicManager = GameMode->FindComponentByClass<URMusicManager>();
	if (!MusicManager) return;

	MusicManager->BeatEvent.AddDynamic(this, &URBeatReaction::Dispatch);
	ChangeTempo(MusicManager->CurrentBPM);
}


void URBeatReaction::Dispatch(int BeatNumber, int Measure, int32 Time, int32 BPM)
{
	if ((One && BeatNumber == 1) ||
		(Two && BeatNumber == 2) ||
		(Three && BeatNumber == 3) ||
		(Four && BeatNumber == 4))
	{
		BeatProgress = 0.0f;
		ChangeTempo(BPM);
		React.Broadcast(BeatNumber, Measure, Time, BPM);
	}
}

void URBeatReaction::ChangeTempo(float BeatsPerMinute)
{
	SecondsPerBeat = 1.0f / (BeatsPerMinute / 60.0f);
}

//@TODO(ejtb): This doesn't need to tick. We can always calculate T from info we get from Dispatch. Optimize if necessary.
void URBeatReaction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BeatProgress += (DeltaTime / SecondsPerBeat);
}
