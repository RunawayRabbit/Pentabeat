#include "RSplineSnake.h"

#include "RBullet.h"
#include "Components/SplineComponent.h"


ARSnek::ARSnek()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline snake component"));
	SetRootComponent(SplineComponent);
}

ARSplineSnake::ARSplineSnake()
{
	PrimaryActorTick.bCanEverTick = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline component"));
	SetRootComponent(Spline);
}

void ARSplineSnake::StartSnake(AActor* InFrom, AActor* InTo)
{
	Spline->ClearSplinePoints();
	From = InFrom;
	To = InTo;
	bSnakeTime = true;

	Spline->AddSplinePoint(From->GetActorLocation(), ESplineCoordinateSpace::World, true);
	Spline->AddSplinePoint(To->GetActorLocation(), ESplineCoordinateSpace::World, true);
}

void ARSplineSnake::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < BulletCount; i++)
	{
		Bullets.Add(GetWorld()->SpawnActor<ARBullet>(BP_BulletClass));
	}
}

void ARSplineSnake::CreateSnek(const FRSnakeData& InSnakeData)
{
	// Create bullet actors
	FRSnakeData SnakeData = InSnakeData;
	
	const FVector StartLoc = SnakeData.FromLoc;
	const FVector TargetLoc = FVector(SnakeData.ToLoc.X, SnakeData.ToLoc.Y, StartLoc.Z);

	const FVector DirToTarget = (TargetLoc - StartLoc).GetSafeNormal2D();

	const float Length = (TargetLoc - StartLoc).Size2D();

	const float PointOffset = Length / static_cast<float>(SnakeData.MiddlePointsCount) + 1.f;
	const FVector Tangent = FVector::CrossProduct(DirToTarget, FVector::ZAxisVector);

	for (int i = 0; i < SnakeData.MiddlePointsCount; i++)
	{
		FVector Point = StartLoc + DirToTarget * PointOffset * i;

		// if first point
		if (i == 0)
		{
			Spline->AddSplinePoint(StartLoc, ESplineCoordinateSpace::World, true);
		}
		else if (i == SnakeData.MiddlePointsCount - 1) // last point
		{
			Spline->AddSplinePoint(TargetLoc, ESplineCoordinateSpace::World, true);
		}

			// if point index even
		else if (i % 2 == 0)
		{
			Point += Tangent * SnakeData.Width;
			Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World, true);
		}
			// if point index uneven
		else
		{
			Point -= Tangent * SnakeData.Width;
			Spline->AddSplinePoint(Point, ESplineCoordinateSpace::World, true);
		}
	}

	// create actor with spline component
	SnakeData.SplineActor = GetWorld()->SpawnActor<ARSnek>();
	auto& SplineComp = SnakeData.SplineActor->SplineComponent;

	// create bullet actors
	for (int i = 0; i < SnakeData.BulletCount; i++)
	{
		SnakeData.Bullets.Add(GetWorld()->SpawnActor<ARBullet>(SnakeData.BulletBPClass));
	}
	
	const float SplineLength = SplineComp->GetSplineLength();
	const float BulletOffset = SplineLength / static_cast<float>(SnakeData.BulletCount);
	for (int i = 0; i < SnakeData.BulletCount; i++)
	{
		const float DistanceAlongSpline = BulletOffset * i;
		const FVector WorldLoc = SplineComp->GetLocationAtDistanceAlongSpline(
			DistanceAlongSpline, ESplineCoordinateSpace::World);
		SnakeData.Bullets[i]->SetActorLocation(WorldLoc);
	}

	SnakeDatas.Add(SnakeData);
}

void ARSplineSnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (bSnakeTime)
	// {
	// 	Spline->ClearSplinePoints();
	// 	const FVector StartLoc = From->GetActorLocation();
	// 	const FVector TargetLoc = FVector(To->GetActorLocation().X, To->GetActorLocation().Y,
	// 	                                  From->GetActorLocation().Z); // give same Z
	// 	const FVector DirToTarget = (TargetLoc - StartLoc).GetSafeNormal();
	//
	// 	HeadLoc = TargetLoc; // * 100.f;
	// 	const float DistToHead = (HeadLoc - StartLoc).Size();
	//
	// 	const float Offset = DistToHead / 3.f;
	// 	FVector SecondPoint = StartLoc + DirToTarget * Offset;
	// 	FVector ThirdPoint = StartLoc + DirToTarget * Offset * 2;
	//
	// 	const auto Tangent = FVector::CrossProduct(DirToTarget, FVector::ZAxisVector);
	//
	// 	Alpha += DeltaTime;
	// 	SecondPoint += Tangent * FMath::Sin(Alpha) * 500.f; //FMath::Sin(Alpha);
	// 	ThirdPoint -= Tangent * FMath::Sin(Alpha) * 500.f;
	//
	// 	//UE_LOG(LogTemp, Warning, TEXT("Alpha: %f"), Alpha);
	//
	// 	Spline->AddSplinePoint(StartLoc, ESplineCoordinateSpace::World, true);
	// 	Spline->AddSplinePoint(SecondPoint, ESplineCoordinateSpace::World, true);
	// 	Spline->AddSplinePoint(ThirdPoint, ESplineCoordinateSpace::World, true);
	// 	Spline->AddSplinePoint(HeadLoc, ESplineCoordinateSpace::World, true);
	//
	// 	
	// }
	//
	//
	// ZeroOne += DeltaTime;
	// if (ZeroOne > 1)
	// {
	// 	ZeroOne -= 1;
	// }
	//
	// // config 0
	// const float SplineLength = Spline->GetSplineLength();
	// const float Offset = SplineLength / static_cast<float>(BulletCount);
	// for (int i = 0; i < BulletCount; i++)
	// {
	// 	const float DistanceAlongSpline = Offset * i;
	// 	const FVector WorldLoc = Spline->GetLocationAtDistanceAlongSpline(
	// 		DistanceAlongSpline, ESplineCoordinateSpace::World);
	// 	Bullets[i]->SetActorLocation(WorldLoc);
	// }
	//



	// --- old ---
	
	// config 1
	// const float SplineLength = Spline->GetSplineLength();
	// const float Offset = SplineLength / static_cast<float>(BulletCount);
	// for (int i = 0; i < BulletCount * ZeroOne; i++)
	// {
	// 	const float DistanceAlongSpline = Offset * i;
	// 	const FVector WorldLoc = Spline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	// 	Bullets[i]->SetActorLocation(WorldLoc);
	// }
	// 
	// config 2
	// const float SplineLength = Spline->GetSplineLength();
	// const float Offset = SplineLength / static_cast<float>(BulletCount);
	// for (int i = 0; i < BulletCount * ZeroOne; i++)
	// {
	// 	const float DistanceAlongSpline = Offset * i;
	// 	const FVector WorldLoc = Spline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	// 	Bullets[i]->SetActorLocation(WorldLoc);
	// }
	// 
	// config 3
	// const float SplineLength = Spline->GetSplineLength();
	// const float Offset = SplineLength / static_cast<float>(BulletCount);
	// for (int i = (BulletCount / 2) * ZeroOne; i < BulletCount * ZeroOne; i++)
	// {
	// 	const float DistanceAlongSpline = Offset * i;
	// 	const FVector WorldLoc = Spline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	// 	Bullets[i]->SetActorLocation(WorldLoc);
	// }
}
