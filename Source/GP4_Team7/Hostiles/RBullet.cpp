#include "RBullet.h"

#include "RBulletPulser.h"
#include "Components/SphereComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ARBullet::ARBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision Component"));
	SphereCollisionComponent->InitSphereRadius(15.f);
	SetRootComponent(SphereCollisionComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereCollisionComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = false; // todo Default to true?
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->bConstrainToPlane = true;
	ProjectileMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
}

void ARBullet::Fire(const FVector& InVelocity, float LifeSpan, TOptional<FRHomingData> HomingData)
{
	if (HomingData.IsSet())
	{
		const auto Data = HomingData.GetValue();
		ProjectileMovementComponent->InitialSpeed = HomingData->InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = HomingData->MaxSpeed;
		ProjectileMovementComponent->bIsHomingProjectile = true;
		ProjectileMovementComponent->HomingAccelerationMagnitude = Data.PullAcceleration;
		ProjectileMovementComponent->HomingTargetComponent = Data.HomingTargetComponent;
		SetLifeSpan(3.f);
	}
	else
	{
		ProjectileMovementComponent->bIsHomingProjectile = false;
		ProjectileMovementComponent->InitialSpeed = InVelocity.Size();
		ProjectileMovementComponent->Velocity = InVelocity;
		SetLifeSpan(LifeSpan);
	}
}

void ARBullet::SetScaleFactor(const float t) const
{
	if (IsActorBeingDestroyed()) return;

	for(auto MeshComponent : MeshComponents)
	{
		const FVector NewScale = UKismetMathLibrary::VLerp(MinScale, MaxScale, t);

		if(IsValid(MeshComponent))
			MeshComponent->SetWorldScale3D(NewScale);
	}
}

void ARBullet::BeginPlay()
{
	Super::BeginPlay();

	const auto GameMode = UGameplayStatics::GetGameMode(AActor::GetWorld());
	if (!GameMode) return;

	const auto BulletPulser = GameMode->FindComponentByClass<URBulletPulser>();
	if (BulletPulser)
	{
		GetComponents<UStaticMeshComponent>(MeshComponents);
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("%d"), MeshComponents.Num()));
		if(MeshComponents.Num() > 0)
		{
			BulletPulser->RegisterMe(this);
		}
	}
}
