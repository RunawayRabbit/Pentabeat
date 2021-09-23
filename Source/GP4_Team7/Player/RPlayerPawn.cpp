#include "GP4_Team7/Player/RPlayerPawn.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP4_Team7/Camera/RCameraSpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RJumpingComponent.h"
#include "RGravityComponent.h"
#include "RMovementComponent.h"
#include "RPelletHandlingComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GP4_Team7/Hostiles/RBoss.h"
#include "GP4_Team7/Hostiles/RBullet.h"
#include "GP4_Team7/Hostiles/RPellet.h"
#include "GP4_Team7/Midi/RBeatReaction.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


ARPlayerPawn::ARPlayerPawn(const FObjectInitializer& ObjectInitializer)
//: Super(ObjectInitializer.SetDefaultSubobjectClass<URMovementComponent>(APawn::))
{
	PrimaryActorTick.bCanEverTick = true;

	// Set up the collision capsule
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ARPlayerPawn::OnBeginOverlap);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	SkeletonMeshPivot = CreateDefaultSubobject<USceneComponent>(TEXT("Skeletal Mesh Pivot"));
	SkeletonMeshPivot->SetupAttachment(RootComponent);

	SpinnyFlip = CreateDefaultSubobject<USceneComponent>(TEXT("SpinnyFlip Pivot"));
	SpinnyFlip->SetupAttachment(SkeletonMeshPivot);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal mesh component"));
	SkeletalMeshComponent->SetupAttachment(SpinnyFlip);

	// Set up the boom arm, range 3 yards, and tell it to use the Pawn's control rotation
	BoomArm = CreateDefaultSubobject<URCameraSpringArmComponent>(TEXT("Boom Arm"));
	BoomArm->SetupAttachment(RootComponent);
	BoomArm->TargetArmLength = 300.0f;
	BoomArm->bUsePawnControlRotation = true;

	// Set up the camera, attach it to the boom arm. Camera doesn't rotate, because the
	// boom is the thing we want to rotate, not the camera.
	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Cam->SetupAttachment(BoomArm, URCameraSpringArmComponent::SocketName);
	Cam->bUsePawnControlRotation = false;

	// Rotating the camera should not rotate the player.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// @TODO(ejtb): Set up movement component?
	//GetMovementComponent()->DoThing(WithThing);

	MovementComponent = CreateDefaultSubobject<URMovementComponent>(TEXT("MovementComponent"));
	JumpingComponent = CreateDefaultSubobject<URJumpingComponent>(TEXT("JumpingComponent"));
	GravityComponent = CreateDefaultSubobject<URGravityComponent>(TEXT("GravityComponent"));
	PelletComponent = CreateDefaultSubobject<URPelletHandlingComponent>(TEXT("PelletHandlingComponent"));

	BeatReacton = CreateDefaultSubobject<URBeatReaction>(TEXT("ActualBeatReactionComponent"));
}


void ARPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState == Dead)
	{
		Velocity = FVector(0.0f, 0.0f, -GravityComponent->Gravity);
		MoveActor(DeltaTime);
		return;
	}

	GroundCheck();

	if (!Velocity.IsNearlyZero())
	{
		MoveActor(DeltaTime);
	}

	if(GetActorLocation().Z < DeathPlaneHeight)
	{
		FallToDeath();
		// Velocity = FVector(0.0f, 0.0f, -GravityComponent->Gravity);
	}

	if(IsInvulnerable)
	{
		UpdateInvulnerabilityStatus(DeltaTime);
	}
}

void ARPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("CamYaw"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("CamPitch"), this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis(TEXT("MoveRight"), MovementComponent, &URMovementComponent::SetHorizontalAxis);
	PlayerInputComponent->BindAxis(TEXT("MoveUp"), MovementComponent, &URMovementComponent::SetVerticalAxis);

	PlayerInputComponent->BindAxis(TEXT("MoveRightGamepad"), MovementComponent, &URMovementComponent::SetGamepadHorizontalAxis);
	PlayerInputComponent->BindAxis(TEXT("MoveUpGamepad"), MovementComponent, &URMovementComponent::SetGamepadVerticalAxis);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, JumpingComponent, &URJumpingComponent::Jump);
	PlayerInputComponent->BindAction(TEXT("ShootPellets"), IE_Pressed, PelletComponent,
	                                 &URPelletHandlingComponent::ShootPellets);

	PlayerInputComponent->BindAction(TEXT("HenrikDebugKey"), IE_Pressed, BossRef, &ARBoss::DebugKeyPress);
}

UCapsuleComponent* ARPlayerPawn::GetCapsuleComponent() const
{
	return CapsuleComponent;
}

void ARPlayerPawn::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ARBullet* bullet = Cast<ARBullet>(OtherActor);

	if (bullet != nullptr)
	{
		GetDamaged();
		OnBulletHit.Broadcast(bullet->GetActorLocation());
		bullet->Destroy();
	}

	ARPellet* pellet = Cast<ARPellet>(OtherActor);
	if (pellet != nullptr)
	{
		PelletComponent->PickupPellet(pellet);
	}
}

void ARPlayerPawn::GroundCheck()
{
	FVector start = GetActorLocation();
	FVector end = start + ((CapsuleComponent->GetScaledCapsuleHalfHeight()) * FVector::DownVector);
	float traceWidth = CapsuleComponent->GetScaledCapsuleRadius();

	FHitResult traceHit;

	bIsOnGround = UKismetSystemLibrary::SphereTraceSingle(this,
	                                                      start,
	                                                      end,
	                                                      traceWidth,
	                                                      GroundQueryType,
	                                                      false,
	                                                      TArray<AActor*>(),
	                                                      EDrawDebugTrace::None,
	                                                      traceHit,
	                                                      true);
}

void ARPlayerPawn::MoveActor(float DeltaTime)
{
	FHitResult sweepHit;

	FVector verticalDelta = FVector::ZeroVector;
	verticalDelta.Z = Velocity.Z * DeltaTime;

	RootComponent->AddRelativeLocation(SkeletonMeshPivot->GetForwardVector() * (MovementComponent->CurrentSpeed * DeltaTime), true,
	                                   &sweepHit);
	RootComponent->AddRelativeLocation(verticalDelta, true, &sweepHit);
}

void ARPlayerPawn::GetDamaged()
{
	if (IsDebugInvulnerable) return;
	if (IsInvulnerable)
		return;

	if (PelletComponent->AttemptPelletBlock())
	{
		BecomeInvulnerable();
		return;
	}

	Die();

	// UGameplayStatics::GetPlayerController(this, 0)->RestartLevel();
}

void ARPlayerPawn::Die()
{
	CurrentState = Dead;
	bIsDead = true;
	MovementComponent->CurrentSpeed = 0.0f;
	Velocity = FVector::ZeroVector;

	OnDeath.Broadcast();
}

void ARPlayerPawn::FallToDeath()
{
	CurrentState = FallingToDeath;
	bIsDead = true;

	OnFallingToDeath.Broadcast();
}

void ARPlayerPawn::UpdateInvulnerabilityStatus(float DeltaTime)
{
	ImmunityTimer -= DeltaTime;

	if(ImmunityTimer <= 0)
	{
		IsInvulnerable = false;
		OnInvulnerabilityEnd.Broadcast(ImmunityTimer);
	}
}

void ARPlayerPawn::BecomeInvulnerable()
{
	ImmunityTimer = HitImmunityDuration;
	IsInvulnerable = true;
	OnInvulnerabilityStart.Broadcast(ImmunityTimer);
}

//
// IPlayerCameraInterface Implementation
//

bool ARPlayerPawn::GotMovementInput()
{
	return MovementComponent->HasReceivedInput();
}
