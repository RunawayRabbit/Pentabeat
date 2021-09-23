// Yeet <3

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "RBeatReaction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FReactEvent, int, BeatNumber, int, Measure, int32, Time, int32, BPM);

class URMusicManager;

UCLASS( Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API URBeatReaction : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URBeatReaction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadOnly)
	float SecondsPerBeat;

	/** Incrementing fractional number of beats since our last beat. */
	UPROPERTY(BlueprintReadOnly)
	float BeatProgress;

public:
	/** Fired when a music beat we care about hits */
	UPROPERTY(BlueprintAssignable)
	FReactEvent React;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Beats")
	bool One;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Beats")
	bool Two;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Beats")
	bool Three;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Beats")
	bool Four;

protected:

	UFUNCTION()
	virtual void Dispatch(int BeatNumber, int Measure, int32 Time, int32 BPM);
	UFUNCTION()
	void ChangeTempo(float BeatsPerMinute);

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
