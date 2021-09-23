#pragma once

#include "CoreMinimal.h"

#include "RJumpingComponent.h"
#include "GameFramework/Pawn.h"
#include "GP4_Team7/Camera/Interfaces/PlayerCharacterInterface.h"


#include "RPlayerPawn.generated.h"

class URBeatReaction;
class ARBoss;
class UInputComponent;
class URCameraSpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;
class URMovementComponent;
class URJumpingComponent;
class URGravityComponent;
class URPelletHandlingComponent;
class USceneComponent;

UENUM()
enum EPawnState
{
	Idle,
	Jumping,
	Dead,
	FallingToDeath
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitDelegate, FVector, BulletLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInvulnerabilityDelegate, float, InvulnerabilityTimeRemaining);

UCLASS()
class GP4_TEAM7_API ARPlayerPawn : public APawn, public IPlayerCharacterInterface
{
	GENERATED_BODY()

private:
	// Debug (todo remove later)
	UPROPERTY(EditDefaultsOnly)
	ARBoss* BossRef;

	//
	// Camera Stuff
	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	URCameraSpringArmComponent* BoomArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCameraComponent* Cam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	URBeatReaction* BeatReacton;

	//
	// IPlayerCameraInterface Implementation
	//

	virtual bool GotMovementInput() override;

	//
	// Capsule Collider
	//

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	// Methods:
public:

	UPROPERTY(Category=Character, EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;

	ARPlayerPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCapsuleComponent* GetCapsuleComponent() const;

	UFUNCTION(BlueprintCallable)
	void GetDamaged();
protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 BodyIndex, bool bFromSweep, const FHitResult& Hit);

	void GroundCheck();
	void MoveActor(float DeltaTime);
	void Die();
	void FallToDeath();
	void UpdateInvulnerabilityStatus(float DeltaTime);
	void BecomeInvulnerable();


	// Variables:
public:
	UPROPERTY()
	bool bIsOnGround = false;

	UPROPERTY()
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> GroundQueryType;

	UPROPERTY()
	TEnumAsByte<EPawnState> CurrentState = Idle;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere)
	float HitImmunityDuration = 0.5f;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 50.0f;

	UPROPERTY(BlueprintAssignable)
	FDeathDelegate OnDeath;

	UPROPERTY(BlueprintAssignable)
	FDeathDelegate OnFallingToDeath;

	UPROPERTY(BlueprintAssignable)
	FInvulnerabilityDelegate OnInvulnerabilityStart;

	UPROPERTY(BlueprintAssignable)
	FInvulnerabilityDelegate OnInvulnerabilityEnd;

	UPROPERTY(BlueprintAssignable)
	FHitDelegate OnBulletHit;

	UPROPERTY(EditAnywhere)
	float DeathPlaneHeight = -300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SkeletonMeshPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SpinnyFlip;

protected:
	UPROPERTY(VisibleAnywhere)
	URMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URJumpingComponent* JumpingComponent;

	UPROPERTY(VisibleAnywhere)
	URGravityComponent* GravityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URPelletHandlingComponent* PelletComponent;

	UPROPERTY()
	float ImmunityTimer = 0.0f;

	UPROPERTY(VisibleAnywhere)
	bool IsInvulnerable = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDebugInvulnerable = false;
};
