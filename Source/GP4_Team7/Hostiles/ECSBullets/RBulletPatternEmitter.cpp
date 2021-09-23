// Yeet <3

#include "RBulletPatternEmitter.h"

#include "Kismet/KismetMathLibrary.h"

ARBulletPatternEmitter::ARBulletPatternEmitter()
{
	PrimaryActorTick.bCanEverTick = true;
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
		TEXT("Instanced Static Mesh Component"));
	SetRootComponent(InstancedStaticMeshComponent);
}

void ARBulletPatternEmitter::BeginPlay()
{
	Super::BeginPlay();
}

void ARBulletPatternEmitter::SpawnCircle(float Radius,
                                         int BulletCount)
{
	const FVector EmitterLoc = GetActorLocation();

	if (BulletCount < 1) return;


	for (int BulletSetIndex = 0; BulletSetIndex < PatternSettings->Settings.NumberOfBulletSets; BulletSetIndex++)
	{
		const float SpreadPerSet = PatternSettings->Settings.SpreadPerSet;
		const float AngleBetweenPoints = SpreadPerSet / static_cast<float>(BulletCount);

		const auto TargetDir = SpawnerForwardDirection; // GetActorLocation().ForwardVector; // todo make into optional parameter
		const float Step = AngleBetweenPoints;

		float Spread = SpreadPerSet;

		TArray<FVector> SpawnDirections;
		const int Start = 0 + (BulletSetIndex * (Spread + PatternSettings->Settings.SpreadBetweenSets));
		const int End = Spread + (BulletSetIndex * (Spread + PatternSettings->Settings.SpreadBetweenSets));

		for (int AngleDeg = Start; AngleDeg < End; AngleDeg += Step)
		{
			FVector SpawnDir = TargetDir.RotateAngleAxis(AngleDeg, FVector::ZAxisVector);

			
			// make it local space to account so we can rotate this Actor to make it spin
			

			SpawnDirections.Add(SpawnDir);
		}

		FRCircle Circle{};
		Circle.Radius = Radius;
		Circle.ExpandSpeed = PatternSettings->Settings.ExpandSpeed;

		for (const auto& SpawnDir : SpawnDirections)
		{
			const FRotator Rot = UKismetMathLibrary::MakeRotFromX(SpawnDir);

			const FVector SpawnPointLocalSpace = SpawnDir;

			Circle.DirectionsToBullets.Add(SpawnDir);

			// DrawDebugLine(GetWorld(), EmitterLoc, SpawnPointLocalSpace, FColor::Emerald, true);

			const FTransform SpawnTransform = FTransform(Rot, SpawnPointLocalSpace);
			const int InstanceId = InstancedStaticMeshComponent->AddInstance(SpawnTransform);

			Circle.MeshInstanceIndices.Add(InstanceId);
		}

		Circles.Add(Circle);
	}
}

void ARBulletPatternEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float SecondsPerBullet = 1.f / PatternSettings->Settings.FireRateBulletsPerSec;

	Timer += DeltaTime;
	if (Timer > SecondsPerBullet)
	{
		Timer = 0.f;
		SpawnCircle(PatternSettings->Settings.SpawnRadius, PatternSettings->Settings.BulletsPerSet);
	}

	UpdateCircleLocs(DeltaTime);

	// spin
	Alpha += DeltaTime; // todo reset

	float YawToAdd;
	if (PatternSettings->Settings.bSinusSpin)
	{
		YawToAdd = FMath::Sin(Alpha) * PatternSettings->Settings.SinusSpinMaxSpeed * DeltaTime;
	}
	else
	{
		YawToAdd = PatternSettings->Settings.SpinSpeed * DeltaTime;
	}
	SpawnerForwardDirection = SpawnerForwardDirection.RotateAngleAxis(YawToAdd, FVector::ZAxisVector);
}

void ARBulletPatternEmitter::UpdateCircleLocs(float DeltaTime)
{
	TArray<FRCircle> ToRemoveList;
	for (auto& Circle : Circles)
	{
		Circle.Radius += DeltaTime * Circle.ExpandSpeed;

		// const bool bShouldDestroy = Circle.Radius > PatternSettings.DespawnRadius;
		// if (bShouldDestroy)
		// {
		// 	// todo destroy
		// }

		// Update each mesh location in the circle
		for (int i = 0; i < Circle.MeshInstanceIndices.Num(); i++)
		{
			InstancedStaticMeshComponent->SelectInstance(true, Circle.MeshInstanceIndices[i], 1);
			FTransform OutInstanceTransform;
			InstancedStaticMeshComponent->GetInstanceTransform(Circle.MeshInstanceIndices[i], OutInstanceTransform);
			
			FVector NewLocalPos = Circle.Radius * Circle.DirectionsToBullets[i];
			OutInstanceTransform.SetLocation(NewLocalPos);
			InstancedStaticMeshComponent->UpdateInstanceTransform(Circle.MeshInstanceIndices[i],
			                                                      OutInstanceTransform);
			InstancedStaticMeshComponent->SelectInstance(false, Circle.MeshInstanceIndices[i], 1);
		}
	}
}
