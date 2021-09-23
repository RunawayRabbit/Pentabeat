#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "RProjectileStructs.h"
#include "GameFramework/Actor.h"

#include "RShockwave.h"

#include "RBoss.generated.h"

class URMusicManager;
class URMusicAsset;
struct FRShockwaveSpawnData;
class ARPellet;
class FOnTimelineVector;
class UTimelineComponent;
class UCurveVector;
class ARSplineSnake;
class URDA_CircularTrackGenerationSettings;
class ARPlayerPawn;

class ARBullet;
class ARBossTrack;


UENUM(BlueprintType)
enum ERBulletSpread
{
	NoSpread UMETA(DisplayName = "No Spread"),
	Spread UMETA(DisplayName = "Spread"),
};

UENUM(BlueprintType)
enum ERProjectileType
{
	BulletHostile UMETA(DisplayName = "Bullet (Hostile)"),
	PelletFriendly UMETA(DisplayName = "Pellet (Friendly)"),
};

UENUM()
enum ERProjectileDestroySetting
{
	ReachedTarget UMETA(DisplayName = "Reached target"),
	ReachedEdge UMETA(DisplayName = "Reached edge of arena"),
};


UENUM(BlueprintType)
enum ERMovementDirection
{
	Clockwise UMETA(DisplayName = "Clockwise"),
	CounterClockwise UMETA(DisplayName = "Counter Clockwise"),
};

UENUM(BlueprintType)
enum ERBossTarget
{
	Center UMETA(DisplayName = "Center"),
	Player UMETA(DisplayName = "Player"),
};

USTRUCT(BlueprintType)
struct FRProjectileSpawnData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARBullet> ProjectileBPClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERBulletSpread> BulletSpread = NoSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERBossTarget> Target = Center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERProjectileDestroySetting> DestroySetting = ReachedTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpreadAngleDeg = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PanningPercentage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPanningDeflectionDeg = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BulletsToSpawn = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// how far out from the boss location the projectile should spawn
	float SpawnOffsetFromBossFirstBullet = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// if there is no spread, bullets will spawn in a line in front of each other before being fired. Set the offset between between them
	float SpawnOffsetFromBossSubsequentBullets = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRHomingData HomingData;
};

USTRUCT(BlueprintType)
struct FRBossMovementData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeightOffsetZ = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERMovementDirection> MovementDirection = Clockwise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ERBossTarget> FaceTarget = Player;
};


// USTRUCT()
// struct FRShockwaveFrameData
// {
// 	GENERATED_USTRUCT_BODY()
//
// 	UPROPERTY()
// 	float InnerRadius = 0.f;
// 	UPROPERTY()
// 	float Width = 1.f;
// 	UPROPERTY()
// 	float Height = 10.f;
// 	UPROPERTY()
// 	float Alpha = 0.f;
// 	UPROPERTY()
// 	bool bTicking = false;
//
// 	float OuterRadius() const
// 	{
// 		return InnerRadius + Width;
// 	}
// };

UCLASS(BlueprintType)
class URDA_PelletSpawnSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ARPellet> PelletBPClass;

	UPROPERTY(EditAnywhere)
	FVector Scale = FVector(1.f, 1.f, 1.f);
	UPROPERTY(EditAnywhere)
	FRotator Rotation = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere)
	float MinDistanceFromBoss = 20.f;
	UPROPERTY(EditAnywhere)
	float Height = 0.f;
	UPROPERTY(EditAnywhere)
	float SpawnEveryXSeconds = 1.f;
	UPROPERTY(EditAnywhere)
	float LifeTime = 10.f;
};

UENUM(BlueprintType)
enum ERBossSpawnType
{
	None UMETA(DisplayName = "None"),
	Projectile UMETA(DisplayName = "Projectile"),
	Shockwave UMETA(DisplayName = "Shockwave"),
};

UENUM(BlueprintType)
enum ERUpdateMovement
{
	KeepCurrent UMETA(DisplayName = "Keep Current"),
	Update UMETA(DisplayName = "Update"),
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMidiEventFiredTestEvent, float, SomeDataPlaceholder);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeath);

// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShockwaveHitPlayer);


UCLASS(BlueprintType, Blueprintable)
class GP4_TEAM7_API ARBoss : public AActor
{
	GENERATED_BODY()

public:
	ARBoss();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceFromCenterOfTheArena = 2000.f; // center of the arena

#pragma region HP
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float Health = 100.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100.f;

	/** Defines which audio + midi should be used for this specific boss */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Music Asset")
	URMusicAsset* MusicAsset;


public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Music Asset")
	void ProcessMarker(const FString& MarkerText, URMusicManager* Manager);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void HealR(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamageR(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthR() const;
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
	FOnBossDeath OnBossDeath;


	void DebugKeyPress();

#pragma endregion

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pellets")
	URDA_PelletSpawnSettings* PelletSpawnSettings;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
	FOnMidiEventFiredTestEvent TestingEventOnMidiEventFired;


#pragma region Actions
	UFUNCTION(BlueprintCallable)
	void UpdateMovement(const FRBossMovementData& InMovementData);

	UFUNCTION(BlueprintCallable)
	void FireProjectile(const FRProjectileSpawnData& InProjectileSpawnData);

	UFUNCTION(BlueprintCallable)
	void FireShockwave(const FRShockwaveSpawnData& ShockwaveSpawnData);

#pragma endregion

public:
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY()
	AActor* ArenaCenter;

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* TimelineComponent;

	// UPROPERTY(EditDefaultsOnly)
	// UStaticMeshComponent* MeshComponentForShockwave;

private:
	// init
	void AddSelfToGameModeTargetingComponent();

	// shockwave
	// UPROPERTY()
	// FRShockwaveSpawnData ShockwaveSpawnData{};
	// UPROPERTY()
	// FRShockwaveFrameData ShockwaveFrameData{};

	void ShockwaveTimelineTick(float DeltaTime);
	void PelletTimelineTick(float DeltaTime);

	void SpawnPelletRandomLoc();
public:
	UPROPERTY(VisibleAnywhere)
	ARSplineSnake* SplineSnake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARSplineSnake> BP_SplineSnakeClass;
private:
	// Helper functions
	ARPlayerPawn* GetPlayer() const;

	void Translate(float DeltaTime);

	float CalculateBulletLifeTime(ERProjectileDestroySetting DestroySetting, const FVector& TargetLocation,
	                              float BulletSpeedXY, float SpawnOffsetFromBoss);

	// todo Implement a NotNormalized variant of this function for better performance
	FVector GetVecToTarget(ERBossTarget Target, bool bNormalized) const;

	// Properties
	UPROPERTY()
	FVector CenterOfArena = FVector::ZeroVector;

	UPROPERTY()
	FRBossMovementData MovementData{};


	UPROPERTY()
	float AngleAroundCenter = 0.f;

	// Shockwave temp
	UPROPERTY()
	float ShockkwaveRadiusInner = 0.f;

#pragma region Testing

	UPROPERTY()
	float PelletSpawnTimer = 0.f;

#pragma endregion
};
