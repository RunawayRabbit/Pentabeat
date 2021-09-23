#include "GP4_Team7/Hostiles/RMeshMusicScalingComponent.h"

#include "GameFramework/GameModeBase.h"
#include "GP4_Team7/Midi/RMusicManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

URMeshMusicScalingComponent::URMeshMusicScalingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URMeshMusicScalingComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> actorStaticMeshes;
	GetOwner()->GetComponents<UStaticMeshComponent>(actorStaticMeshes);
	for (UStaticMeshComponent* meshComp : actorStaticMeshes)
	{
		if(meshComp == nullptr)
			continue;

		FScalingMesh newMesh;
		newMesh.OriginalScale = meshComp->GetComponentScale();
		newMesh.Mesh = meshComp;
		if(meshComp->ComponentHasTag(TEXT("ScaleOnBeat")))
			ScaleOnBeatMeshes.Add(newMesh);
		if(meshComp->ComponentHasTag(TEXT("ScaleOnNote")))
			ScaleOnNoteMeshes.Add(newMesh);
	}

	const auto GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (!GameMode) return;

	const auto MusicManager = GameMode->FindComponentByClass<URMusicManager>();
	if (!MusicManager) return;

	MusicManager->MusicEvent.AddDynamic(this, &URMeshMusicScalingComponent::OnMusicEvent);
}


void URMeshMusicScalingComponent::ScaleUpMeshes(TArray<FScalingMesh>& Meshes)
{
	for(FScalingMesh& mesh : Meshes)
	{
		FVector newScale = mesh.OriginalScale;
		newScale *= MeshScaleMultiplier;
		mesh.Mesh->SetWorldScale3D(newScale);

		mesh.BeatScaleSize = newScale;
		mesh.LerpAlphaDownscale = 0.0f;
	}
}

void URMeshMusicScalingComponent::GraduallyScaleDownMeshes(float DownscaleLerpSpeed, float DeltaTime)
{
	for(FScalingMesh& mesh : ScaleOnBeatMeshes)
	{
		if(mesh.LerpAlphaDownscale >= 1.0f)
			continue;

		mesh.LerpAlphaDownscale += DeltaTime * DownscaleLerpSpeed;
		mesh.LerpAlphaDownscale = FMath::Clamp(mesh.LerpAlphaDownscale, 0.0f, 1.0f);
		FVector lerpedScale = UKismetMathLibrary::VLerp(mesh.BeatScaleSize, mesh.OriginalScale, mesh.LerpAlphaDownscale);
		mesh.Mesh->SetWorldScale3D(lerpedScale);
	}

	for(FScalingMesh& mesh : ScaleOnNoteMeshes)
	{
		if(mesh.LerpAlphaDownscale >= 1.0f)
			continue;

		mesh.LerpAlphaDownscale += DeltaTime * DownscaleLerpSpeed;
		mesh.LerpAlphaDownscale = FMath::Clamp(mesh.LerpAlphaDownscale, 0.0f, 1.0f);
		FVector lerpedScale = UKismetMathLibrary::VLerp(mesh.BeatScaleSize, mesh.OriginalScale, mesh.LerpAlphaDownscale);
		mesh.Mesh->SetWorldScale3D(lerpedScale);
	}
}

void URMeshMusicScalingComponent::OnMusicEvent(FMusicEventInfo MusicEventInfo)
{
	if(MusicEventInfo.Type == ERMusicEventType::Beat)
	{
		FramesSinceLastBeat = 0;
	}

	if(MusicEventInfo.Type == ERMusicEventType::NotePlayed)
	{
		ScaleUpMeshes(ScaleOnNoteMeshes);
		if(FramesSinceLastBeat <= 1)
		{
			ScaleUpMeshes(ScaleOnBeatMeshes);
		}
	}
}

void URMeshMusicScalingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GraduallyScaleDownMeshes(MeshScaleDownSpeed, DeltaTime);
	FramesSinceLastBeat++;
}

