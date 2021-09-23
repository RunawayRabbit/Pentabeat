
#include "RPellet.h"

#include "GP4_Team7/Midi/RBeatReaction.h"
#include "Kismet/KismetMathLibrary.h"

ARPellet::ARPellet()
{
	BeatReaction = CreateDefaultSubobject<URBeatReaction>(TEXT("Beat Reactor Component"));
	PrimaryActorTick.bCanEverTick = true;
}

void ARPellet::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();

	BaseScale = StaticMeshComponent->GetComponentScale();
}

void ARPellet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float LifespanBeats = GetLifeSpan() / BeatReaction->SecondsPerBeat;

	BeatReaction->One = (LifespanBeats < StartFlashingOnes);

	BeatReaction->Three = (LifespanBeats < StartFlashingTwos);

	BeatReaction->Two = (LifespanBeats < StartFlashingThrees);
	BeatReaction->Four = (LifespanBeats < StartFlashingFours);

	const auto BigScale = BaseScale * ScaleFactor;
	const auto T = FMath::Clamp(BeatReaction->BeatProgress, 0.0f, 1.0f);
	const auto NewScale = UKismetMathLibrary::VLerp(BigScale, BaseScale, T);
	StaticMeshComponent->SetWorldScale3D(NewScale);
}

void ARPellet::SetLifeSpan(float InLifespan)
{
	const float LifespanBeats = InLifespan / BeatReaction->SecondsPerBeat;
	const float LifespanMeasures = LifespanBeats / 4.0f;
	const float NewMeasures = FMath::CeilToFloat(LifespanMeasures);
	const float NewBeats = NewMeasures * 4.0f;

	Super::SetLifeSpan(NewBeats * BeatReaction->SecondsPerBeat);
}
