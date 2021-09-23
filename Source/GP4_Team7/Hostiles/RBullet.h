#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RProjectileStructs.h"
#include "RBullet.generated.h"


class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class GP4_TEAM7_API ARBullet : public AActor // this is the base bullet: for bullets with a constant velocity
{
	GENERATED_BODY()

public:
	ARBullet();

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	USphereComponent* SphereCollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	void Fire(const FVector& InVelocity, float LifeSpan, TOptional<FRHomingData> HomingData);

	UFUNCTION()
	void SetScaleFactor(const float t) const;

	UPROPERTY(EditAnywhere, Category = "Beat Pulse")
	FVector MinScale = FVector::OneVector;
	UPROPERTY(EditAnywhere, Category = "Beat Pulse")
	FVector MaxScale = FVector::OneVector * 1.2f;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<UStaticMeshComponent*> MeshComponents;
};
