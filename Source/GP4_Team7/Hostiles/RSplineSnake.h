#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSplineSnake.generated.h"

class USplineComponent;
class ARBullet;

UCLASS()
class GP4_TEAM7_API ARSnek : public AActor
{
public:
	GENERATED_BODY()
	ARSnek();
	
	UPROPERTY()
	USplineComponent* SplineComponent;
};

USTRUCT(BlueprintType)
struct FRSnakeData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARBullet> BulletBPClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FromLoc = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ToLoc = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MiddlePointsCount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Width = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BulletCount = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 100.f;
	UPROPERTY()
	TArray<ARBullet*> Bullets;

	UPROPERTY()
	ARSnek* SplineActor;
};



UCLASS(BlueprintType, Blueprintable)
class GP4_TEAM7_API ARSplineSnake : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARSplineSnake();

	UPROPERTY()
	USplineComponent* Spline;

	void StartSnake(AActor* InFrom, AActor* InTo);

	UFUNCTION(BlueprintCallable)
	void CreateSnek(const FRSnakeData& InSnakeData);

	UPROPERTY()
	TArray<FRSnakeData> SnakeDatas;
	
	UPROPERTY()
	AActor* From;
	UPROPERTY()
	AActor* To;

	UPROPERTY()
	float Alpha = 0.f;

	UPROPERTY(EditAnywhere)
	float ForwardSpeed = 100.f;

	UPROPERTY()
	bool bSnakeTime = false;

	UPROPERTY()
	FVector HeadLoc = FVector::ZeroVector;

	UPROPERTY()
	float HeadOffset = 0.f;

	UPROPERTY()
	float DistanceAlpha = 0.f;

	UPROPERTY()
	int BulletCount = 30;

	UPROPERTY()
	TArray<ARBullet*> Bullets;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARBullet> BP_BulletClass;

	UPROPERTY()
	float ZeroOne = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
