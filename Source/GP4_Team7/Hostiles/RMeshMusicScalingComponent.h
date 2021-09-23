// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GP4_Team7/Midi/FMusicEventInfo.h"
#include "GP4_Team7/Midi/RBeatReaction.h"

#include "RMeshMusicScalingComponent.generated.h"

class UStaticMeshComponent;

USTRUCT()
struct FScalingMesh
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY()
	FVector OriginalScale = FVector::OneVector;
	UPROPERTY()
	FVector BeatScaleSize = FVector::OneVector;
	UPROPERTY()
	float LerpAlphaDownscale = 1.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API URMeshMusicScalingComponent : public UActorComponent
{
	GENERATED_BODY()

	//Methods:
public:
	URMeshMusicScalingComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
	void GraduallyScaleDownMeshes(float DownscaleLerpSpeed, float DeltaTime);

	//Variables:
public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MeshScaleMultiplier = 1.1f;

	UPROPERTY(EditDefaultsOnly)
	float MeshScaleDownSpeed = 0.5f;

	UPROPERTY()
	TArray<FScalingMesh> ScaleOnNoteMeshes;
	TArray<FScalingMesh> ScaleOnBeatMeshes;

	void ScaleUpMeshes(TArray<FScalingMesh>& Meshes);

	UFUNCTION()
	void OnMusicEvent(FMusicEventInfo MusicEventInfo);

	int FramesSinceLastBeat;
};


