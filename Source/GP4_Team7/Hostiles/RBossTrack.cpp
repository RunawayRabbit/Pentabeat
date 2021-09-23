#include "RBossTrack.h"

#include "Components/SplineComponent.h"

bool operator==(const FRSplinePointData& Left, const FRSplinePointData& Right)
{
	// const bool bEqualLoc = Left.WorldLoc == Right.WorldLoc;
	return Left.bDoSomething == Right.bDoSomething;
}

ARBossTrack::ARBossTrack()
{
	PrimaryActorTick.bCanEverTick = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SetRootComponent(SplineComponent);
}

void ARBossTrack::BeginPlay()
{
	Super::BeginPlay();
}

void ARBossTrack::GenerateCircularTrack(const URDA_CircularTrackGenerationSettings* CircularTrackSettings)
{
	SplineComponent->ClearSplinePoints();
	
	constexpr float Tau = PI * 2.f;
	constexpr int SegmentCount = 4; // 4;
	// SegmentCount += 1; // since the start and end point is the same

	// even angles to make a circle
	constexpr float AngleBetweenEachPoint = Tau / SegmentCount;
	// const float Rest = Tau - AngleBetweenEachPoint;
	// const float AddToEach = Rest / SegmentCount;
	// AngleBetweenEachPoint += AddToEach;

	

	TArray<float> Angles;
	for (int i = 0; i < Tau; i += AngleBetweenEachPoint)
	{
		Angles.Add(i);
	}

	UE_LOG(LogTemp, Warning, TEXT("Point count: %f"), Angles.Num());

	const FVector CenterPoint = GetActorLocation();
	const float Radius = CircularTrackSettings->Radius;

	TArray<FVector> SplinePointLocations;
	for (const auto Angle : Angles)
	{
		const auto X = CenterPoint.X + Radius * FMath::Cos(Angle);
		const auto Y = CenterPoint.Y + Radius * FMath::Sin(Angle);
		const FVector PointWorldSpace = FVector(X, Y, CenterPoint.Z);
		SplinePointLocations.Add(PointWorldSpace);
	}

	TArray<FSplinePoint> SplinePoints;
	for (int i = 0; i < SplinePointLocations.Num(); i++)
	{
		// const auto SplinePoint = FSplinePoint(static_cast<float>(i), SplinePointLocations[i]);
		SplineComponent->AddSplinePoint(SplinePointLocations[i], ESplineCoordinateSpace::World);
	}

	SplineComponent->SetClosedLoop(true);
}

void ARBossTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
