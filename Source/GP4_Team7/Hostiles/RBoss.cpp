#include "RBoss.h"

#include "DrawDebugHelpers.h"
#include "RBullet.h"
#include "RPellet.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GP4_Team7/GameModes/RPlayerTargetingComponent.h"
#include "GP4_Team7/Player/RPlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ARBoss::ARBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));
}

void ARBoss::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	CenterOfArena = GetActorLocation();

	ArenaCenter = GetWorld()->SpawnActor<AActor>(CenterOfArena, FRotator::ZeroRotator);

	//@TODO(ejtb): Temporarily disabled the snek. We can turn it on later!
	//SplineSnake = GetWorld()->SpawnActor<ARSplineSnake>(BP_SplineSnakeClass);
	//SplineSnake->StartSnake(this, ArenaCenter);

	AddSelfToGameModeTargetingComponent();
}

void ARBoss::AddSelfToGameModeTargetingComponent()
{
	const AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	UActorComponent* ActorComponent = GameMode->GetComponentByClass(URPlayerTargetingComponent::StaticClass());
	URPlayerTargetingComponent* TargetingComponent = Cast<URPlayerTargetingComponent>(ActorComponent);

	if (TargetingComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("GameMode does not have RPlayerTargetingComponent, Boss cannot add reference to itself"));
		return;
	}
	TargetingComponent->PlayerTarget = this;
	UE_LOG(LogTemp, Log, TEXT("PlayerTarget is now %s"), *GetName());
}

void ARBoss::ProcessMarker_Implementation(const FString& MarkerText, URMusicManager* Manager)
{
}

void ARBoss::HealR(float Amount)
{
	Health += Amount;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

void ARBoss::TakeDamageR(float Amount)
{
	Health -= Amount;
	if (Health <= 0)
	{
		OnBossDeath.Broadcast();
	}
}

float ARBoss::GetHealthR() const
{
	return Health;
}

#pragma region Actions

void ARBoss::DebugKeyPress()
{
}

void ARBoss::UpdateMovement(const FRBossMovementData& InMovementData)
{
	MovementData = InMovementData;
}

void ARBoss::SpawnPelletRandomLoc()
{
	const auto SpawnLocXY = FMath::RandPointInCircle(DistanceFromCenterOfTheArena - PelletSpawnSettings->MinDistanceFromBoss);
	const auto SpawnLoc = FVector(SpawnLocXY.X, SpawnLocXY.Y, CenterOfArena.Z + PelletSpawnSettings->Height);
	//const FTransform SpawnTransf = FTransform(PelletSpawnSettings->Rotation, SpawnLoc);

	const auto Pellet = GetWorld()->SpawnActor<ARPellet>(PelletSpawnSettings->PelletBPClass); // , SpawnTransf);
	if(Pellet)
	{
		Pellet->SetLifeSpan(PelletSpawnSettings->LifeTime);
		Pellet->SetActorLocationAndRotation(SpawnLoc, PelletSpawnSettings->Rotation);
	}
}

void ARBoss::FireProjectile(const FRProjectileSpawnData& InProjectileSpawnData)
{
	// TEnumAsByte<EProjectileType> ProjectileType = BulletHostile;
	// TEnumAsByte<EBulletSpread> BulletSpread = NoSpread;
	// int SpreadAngleRad = 4.f / (PI * 2.f); // 20.f; // angle between each bullet getting shot out. Only used if NumOfBulletsSpread > 1.
	// int BulletsToSpawn = 0;
	// float Speed = 400.f;
	// todo: bool bHoming = false;


	// todo [Optimization] Use squared distances for comparisons


	const auto ThisLoc = GetActorLocation();
	const auto DirToTargetRaw = GetVecToTarget(InProjectileSpawnData.Target, true);

	// Rotate the input spread based on panning
	const auto PanningDeflectionAngle = InProjectileSpawnData.PanningPercentage * InProjectileSpawnData.MaxPanningDeflectionDeg;
	const auto DirToTarget = UKismetMathLibrary::RotateAngleAxis(DirToTargetRaw, PanningDeflectionAngle, FVector::UpVector);

	switch (InProjectileSpawnData.BulletSpread)
	{
	case NoSpread:
		{
			const auto Rot = UKismetMathLibrary::MakeRotFromX(DirToTarget);
			const auto DirToTargetXY = FVector(DirToTarget.X, DirToTarget.Y, 0.f);
			const auto FireVelocity = InProjectileSpawnData.Speed * DirToTargetXY;

			const float SubsequentBulletOffset = InProjectileSpawnData.SpawnOffsetFromBossSubsequentBullets;

			// process each bullet
			for (int i = 0; i < InProjectileSpawnData.BulletsToSpawn; i++)
			{
				// calculate the offset the bullet
				const float Offset = InProjectileSpawnData.SpawnOffsetFromBossFirstBullet +
					(SubsequentBulletOffset * i);
				const FVector SpawnLoc = ThisLoc + (DirToTargetXY * Offset);
				const auto SpawnTransform = FTransform(Rot, SpawnLoc);


				// calculate the lifetime the bullet will have
				const float BulletSpeedXY = FireVelocity.Size2D();
				float LifeTime = 0.0f;
				if (InProjectileSpawnData.Target == Player)
				{
					const auto TargetLoc = GetPlayer()->GetActorLocation();

					LifeTime = CalculateBulletLifeTime(InProjectileSpawnData.DestroySetting, TargetLoc,
					                                   BulletSpeedXY, Offset);
				}
				else if (InProjectileSpawnData.Target == Center)
				{
					const auto TargetLoc = CenterOfArena;

					LifeTime = CalculateBulletLifeTime(InProjectileSpawnData.DestroySetting, TargetLoc,
					                                   BulletSpeedXY, Offset);
				}

				// spawn the bullet
				const auto Bullet = GetWorld()->SpawnActor<ARBullet>(InProjectileSpawnData.ProjectileBPClass,
				                                                     SpawnTransform);

				if (InProjectileSpawnData.HomingData.bHoming)
				{
					if (InProjectileSpawnData.Target == Center)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
						                                 TEXT("RBoss: Center can not be homing target"));
						return;
					}

					FRHomingData HomingData{};
					HomingData.PullAcceleration = InProjectileSpawnData.HomingData.PullAcceleration;
					HomingData.HomingTargetComponent = GetPlayer()->SkeletalMeshComponent;

					Bullet->Fire(FireVelocity, LifeTime, TOptional<FRHomingData>(HomingData));
				}
				else
				{
					TOptional<FRHomingData> HomingData = TOptional<FRHomingData>();
					Bullet->Fire(FireVelocity, LifeTime, HomingData);
				}
			}
		}
		break;
	case Spread:
		{
			const FVector DirToCenterRaw = (CenterOfArena - GetActorLocation()).GetSafeNormal2D();

			// Rotate the input spread based on panning
			const auto DirToCenter = UKismetMathLibrary::RotateAngleAxis(DirToCenterRaw, PanningDeflectionAngle, FVector::UpVector);

			const float Step = InProjectileSpawnData.SpreadAngleDeg;
			const float TotalSpread = Step * (InProjectileSpawnData.BulletsToSpawn - 1);
			float HalfTotalSpread = TotalSpread / 2.f;

			// If the bullet count is even, there can't be a shot straight towards the boss if all of the shots
			// are going to fit within the given spread
			if (InProjectileSpawnData.BulletsToSpawn % 2 == 0)
			{
				HalfTotalSpread += Step / 4.f; // Half a step / 2 since it's for half of the total spread
			}

			TArray<FVector> FireDirections;
			for (int AngleDeg = -HalfTotalSpread; AngleDeg <= HalfTotalSpread; AngleDeg += Step)
			{
				const FVector FireDir = DirToCenter.RotateAngleAxis(AngleDeg, FVector::ZAxisVector);
				FireDirections.Add(FireDir);

				if (InProjectileSpawnData.bDebug)
				{
					DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FireDir * 5000.f, FColor::Cyan,
					              false,
					              .5);
				}
			}

			for (const auto& FireDir : FireDirections)
			{
				const FRotator Rot = UKismetMathLibrary::MakeRotFromX(FireDir);
				const FVector SpawnPointWorldSpace = ThisLoc + (FireDir * InProjectileSpawnData.
					SpawnOffsetFromBossFirstBullet);
				const auto SpawnTransform = FTransform(Rot, SpawnPointWorldSpace);

				const auto Bullet = GetWorld()->SpawnActor<ARBullet>(InProjectileSpawnData.ProjectileBPClass,
				                                                     SpawnTransform);

				const auto FireVelocity = FireDir * InProjectileSpawnData.Speed;

				const float LifeTime = CalculateBulletLifeTime(ReachedEdge, SpawnPointWorldSpace,
				                                               InProjectileSpawnData.Speed, 1.f);

				Bullet->Fire(FireVelocity, LifeTime, TOptional<FRHomingData>());
			}
		}
		break;
	default: UE_LOG(LogTemp, Error, TEXT("Action for bullet spread option not set up."));
	}
}

void ARBoss::FireShockwave(const FRShockwaveSpawnData& ShockwaveSpawnData)
{
	const auto Shockwave = GetWorld()->SpawnActor<ARShockwave>(ShockwaveSpawnData.ShockwaveBPClass);
	const float Scale = ShockwaveSpawnData.Scale;
	Shockwave->SetActorScale3D(FVector(Scale, Scale, Scale));
	Shockwave->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + ShockwaveSpawnData.SpawnZOffset));
	Shockwave->FireShockwave(ShockwaveSpawnData);
}

// old Shockwave // void ARBoss::FireShockwave(const FRShockwaveSpawnData& InShockwaveSpawnData)
// {
// 	OnFireShockwave.Broadcast(InShockwaveSpawnData);
// 	// UE_LOG(LogTemp, Warning, TEXT("Spawning shockwave.."))
// 	// ShockwaveFrameData = FRShockwaveFrameData{};
// 	// ShockwaveFrameData.InnerRadius = 0.f;
// 	// ShockwaveFrameData.Width = InShockwaveSpawnData.ZoneWidth;
// 	// ShockwaveFrameData.Height = GetActorLocation().Z + InShockwaveSpawnData.Height;
// 	// ShockwaveFrameData.Alpha = 0.f;
// 	// ShockwaveFrameData.bTicking = true;
// }

// void ARBoss::ShockwaveTimelineTick(float DeltaTime)
// {
// 	if (!ShockwaveFrameData.bTicking)
// 	{
// 		return;
// 	}
// 	if (ShockwaveFrameData.Alpha > 1)
// 	{
// 		ShockwaveFrameData.bTicking = false;
// 		return;
// 	}
//
// 	ShockwaveFrameData.Alpha += DeltaTime;
// 	ShockwaveFrameData.InnerRadius = ShockwaveFrameData.Alpha * (DistanceFromCenterOfTheArena * 2);
//
// 	constexpr int CircleSegments = 100;
// 	constexpr float DebugLifeTime = .1f;
// 	const FVector YAxis = FVector::XAxisVector;
// 	const FVector ZAxis = FVector::YAxisVector;
//
// 	DrawDebugCircle(GetWorld(), GetActorLocation(), ShockwaveFrameData.InnerRadius, CircleSegments, FColor::Orange,
// 	                false, DebugLifeTime, 0, 2.f, YAxis, ZAxis);
// 	DrawDebugCircle(GetWorld(), GetActorLocation(), ShockwaveFrameData.OuterRadius(), CircleSegments, FColor::Orange,
// 	                false, DebugLifeTime, 0, 2.f, YAxis, ZAxis);
//
//
// 	const FVector PlayerLoc = GetPlayer()->GetActorLocation();
// 	const FVector ThisLoc = GetActorLocation();
// 	const float DistToPlayer = FVector::Dist2D(PlayerLoc, ThisLoc);
// 	if (DistToPlayer > ShockwaveFrameData.InnerRadius && DistToPlayer < ShockwaveFrameData.OuterRadius())
// 	{
// 		if (PlayerLoc.Z < ShockwaveFrameData.Height)
// 		{
// 			GetPlayer()->GetDamaged();
// 		}
// 	}
// }
#pragma endregion

void ARBoss::PelletTimelineTick(float DeltaTime)
{
	if (!PelletSpawnSettings) return;

	PelletSpawnTimer += DeltaTime;
	if (PelletSpawnTimer > 1.f / PelletSpawnSettings->SpawnEveryXSeconds)
	{
		PelletSpawnTimer = 0.f;
		SpawnPelletRandomLoc();
	}
}

void ARBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Translate(DeltaTime);

	PelletTimelineTick(DeltaTime);

	// ShockwaveTimelineTick(DeltaTime);
}

ARPlayerPawn* ARBoss::GetPlayer() const
{
	return Cast<ARPlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

FVector ARBoss::GetVecToTarget(ERBossTarget Target, bool bNormalized) const
{
	switch (Target)
	{
	case Center:
		{
			const FVector VecToCenter = CenterOfArena - GetActorLocation();
			if (bNormalized)
			{
				return VecToCenter.GetSafeNormal();
			}
			return VecToCenter;
		}
	case Player:
		{
			const FVector VecToPlayer = GetPlayer()->GetActorLocation() - GetActorLocation();
			if (bNormalized)
			{
				return VecToPlayer.GetSafeNormal();
			}
			return VecToPlayer;
		}
	default:
		{
			UE_LOG(LogTemp, Error,
			       TEXT("There's no implementation for getting the direction towards that ERProjectileTarget."))
			return FVector::ZeroVector;
		}
	}
}

void ARBoss::Translate(float DeltaTime)
{
	// **
	// Location
	// **
	constexpr float Tau = PI * 2.f;

	// const float SplineLength = BossTrack->SplineComponent->GetSplineLength();

	// todo MovementData.Speed

	const float Speed = (MovementData.Speed * 0.01f) / Tau;

	switch (MovementData.MovementDirection)
	{
	case Clockwise: AngleAroundCenter += Speed * DeltaTime;
		break;
	case CounterClockwise: AngleAroundCenter -= Speed * DeltaTime;
		break;
	}

	const auto& Center = CenterOfArena;
	const auto& Radius = DistanceFromCenterOfTheArena;

	if (AngleAroundCenter > Tau)
	{
		AngleAroundCenter -= Tau;
	}
	else if (AngleAroundCenter < 0)
	{
		AngleAroundCenter += Tau;
	}

	// calculate new location
	const float X = Center.X + Radius * FMath::Cos(AngleAroundCenter);
	const float Y = Center.Y + Radius * FMath::Sin(AngleAroundCenter);
	const float Z = Center.Z;

	const auto NewLoc = FVector(X, Y, Z + MovementData.HeightOffsetZ);

	SetActorLocation(NewLoc);

	// **
	// Rotation
	// **
	switch (MovementData.FaceTarget)
	{
	case ERBossTarget::Player:
		{
			const FRotator NewRot = UKismetMathLibrary::MakeRotFromX(GetVecToTarget(ERBossTarget::Player, false));
			SetActorRotation(NewRot);
		}
		break;
	case ERBossTarget::Center:
		{
			const FRotator NewRot = UKismetMathLibrary::MakeRotFromX(GetVecToTarget(ERBossTarget::Center, false));
			SetActorRotation(NewRot);
		}
		break;
	default:
		{
		}
	}
}

float ARBoss::CalculateBulletLifeTime(ERProjectileDestroySetting DestroySetting, const FVector& TargetLocation,
                                      float BulletSpeedXY, float SpawnOffsetFromBoss)
{
	// Given a point P0 on a circle, a circle center point C, a point inside the circle T,
	// The point P1 is positioned on the "other side", where the line that goes between P0 and T intersects the circle again.

	const FVector P0 = FVector(GetActorLocation().X, GetActorLocation().Y, 0.f);
	const FVector C = CenterOfArena;
	const FVector T = TargetLocation;

	const FVector DirToCenter = (C - P0).GetSafeNormal2D();
	const FVector DirToTarget = (T - P0).GetSafeNormal2D();


	if (DestroySetting == ReachedTarget)
	{
		const float DistToTarget = (T - P0).Size2D() - SpawnOffsetFromBoss;
		return DistToTarget / BulletSpeedXY;
	}

	if (DestroySetting == ReachedEdge)
	{
		const float Radius = (C - P0).Size2D();
		const float DistToCenter = Radius - SpawnOffsetFromBoss;
		// todo Should offset be added on the next line instead?
		const float Diameter = DistToCenter * 2.f;

		if (TargetLocation == C)
		{
			const float LifeTime = Diameter / BulletSpeedXY;
			return LifeTime;
		}

		// The dot product between P0->C and P0->T
		const float Dot = FVector::DotProduct(DirToCenter, DirToTarget);

		const float DistanceLeftToP1 = Diameter * Dot;
		const float LifeTime = DistanceLeftToP1 / BulletSpeedXY;
		return LifeTime;
	}

	UE_LOG(LogTemp, Warning, TEXT("DestroySetting lifetime calculation not implemented."))
	return 0;
}

// ******** OLD CODE *******

// float ARBoss::CalculateBulletLifeTime(ERProjectileDestroySetting DestroySetting, ERBossTarget Target,
//                                       float BulletSpeedXY, float DistanceToCenterXY)
// {
// 	switch (DestroySetting)
// 	{
// 	case ReachedTarget:
// 		{
// 			return DistanceToCenterXY / BulletSpeedXY; // the time it takes to reach the center
// 		}
// 	case ReachedEdge:
// 		{
// 			if (Target == Center)
// 			{
// 				const float DistToOtherSideXY = DistanceToCenterXY * 2.f;
// 				return DistToOtherSideXY / BulletSpeedXY;
// 			}
//
// 			if (Target == Player)
// 			{
// 				const auto DirToCenter = GetVecToTarget(Center, true);
// 				const auto DirToPlayer = GetVecToTarget(Player, true);
//
// 				const auto DirToCenterXY = FVector(DirToCenter.X, DirToCenter.Y, 0.f);
// 				const auto DirToPlayerXY = FVector(DirToPlayer.X, DirToPlayer.Y, 0.f);
//
// 				const float Dot = FVector::DotProduct(DirToCenterXY, DirToPlayerXY);
//
// 				const float DistToOtherSideXY = DistanceToCenterXY * 2.f;
// 				const float DistLeftToOtherSideXY = DistToOtherSideXY * Dot;
// 				return DistLeftToOtherSideXY / BulletSpeedXY;
// 			}
// 		}
// 	}
//
// 	UE_LOG(LogTemp, Error, TEXT("RBoss: Implementation lacking for that target."))
// 	return 0;
// }


// Generate track if necessary
// if (TrackType == CustomBP)
// {
// 	if (!BossTrack)
// 	{
// 		UE_LOG(LogTemp, Warning,
// 		       TEXT(
// 			       "If you want to use a custom track for the Boss, you need to provide it's blueprint in the boss settings."
// 		       ))
// 	}
// }
// else if (TrackType == CircularGenerated)
// {
// 	const FVector SpawnLoc = CenterOfArena;
// 	const FRotator SpawnRot = GetActorRotation();
// 	const FTransform SpawnTransform = FTransform(SpawnRot, SpawnLoc);
//
// 	BossTrack = GetWorld()->SpawnActor<ARBossTrack>(BossTrackBpClass, SpawnTransform);
// 	BossTrack->GenerateCircularTrack(CircularTrackSettings);
// }
