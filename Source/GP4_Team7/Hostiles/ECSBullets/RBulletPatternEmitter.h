// Yeet <3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RBulletPatternEmitter.generated.h"

class ARBullet;
using RBulletEntity = uint32;
constexpr RBulletEntity MAX_ENTRIES = 5000;

USTRUCT()
struct FRCircle
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY()
	float Radius;
	UPROPERTY()
	int DirectionsCount;
	UPROPERTY()
	TArray<FVector> DirectionsToBullets;
	UPROPERTY()
	TArray<int> MeshInstanceIndices;
	UPROPERTY()
	float ExpandSpeed;
};

USTRUCT(BlueprintType)
struct FRBulletPatternSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float FireRateBulletsPerSec = 3.f;

	UPROPERTY(EditAnywhere)
	float ExpandSpeed = 500.f;

	UPROPERTY(EditAnywhere)
	float SpawnRadius = 100.f;

	UPROPERTY(EditAnywhere)
	float DespawnRadiusTODO = 1000.f; // todo implement
	
	UPROPERTY(EditAnywhere)
	int BulletsPerSet = 4.f;

	UPROPERTY(EditAnywhere)
	int NumberOfBulletSets = 2;

	UPROPERTY(EditAnywhere)
	float SpreadPerSet = 90.f;

	UPROPERTY(EditAnywhere)
	float SpreadBetweenSets = 90.f;

	UPROPERTY(EditAnywhere)
	float SpinSpeed = 30.f;

	UPROPERTY(EditAnywhere)
	bool bSinusSpin = false;

	UPROPERTY(EditAnywhere)
	float SinusSpinMaxSpeed = 100.f;
};

UCLASS(BlueprintType)
class GP4_TEAM7_API URDA_BulletPatternSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRBulletPatternSettings Settings;
};

UCLASS(BlueprintType, Blueprintable) // for now at least
class GP4_TEAM7_API ARBulletPatternEmitter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARBulletPatternEmitter();
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<ARBullet> BulletBPClass;
	
	UPROPERTY()
	TArray<FRCircle> Circles;

	// UPROPERTY(EditAnywhere)
	// FRBulletPatternSettings PatternSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	URDA_BulletPatternSettings* PatternSettings;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;
	
private:
	UPROPERTY()
	FVector SpawnerForwardDirection = FVector::XAxisVector; // by rotating this, I can easily rotate the spawn pattern
	
	float Timer = 0.f;
	float Alpha = -1.f;
	
protected:
	virtual void BeginPlay() override;

	// Circles
	UFUNCTION(BlueprintCallable)
	void SpawnCircle(float Radius, int BulletCount);

	UFUNCTION(BlueprintCallable)
	void UpdateCircleLocs(float DeltaTime);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
