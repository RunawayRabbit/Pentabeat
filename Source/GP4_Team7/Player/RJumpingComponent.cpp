#include "GP4_Team7/Player/RJumpingComponent.h"
#include "RPlayerPawn.h"

URJumpingComponent::URJumpingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URJumpingComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ARPlayerPawn>(GetOwner());
}

void URJumpingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(JumpTimer < JumpDuration)
	{
		JumpTimer += DeltaTime;
		SetJumpVelocity(DeltaTime);
	}
	else if(Owner->CurrentState == Jumping
		&& Owner->bIsOnGround)
	{
		Owner->CurrentState = Idle; //TODO: switch to different states
		OnJumpLand.Broadcast();
	}

	
	if(JumpGraceTimer < JumpGracePeriod)
	{
		JumpGraceTimer += DeltaTime;
		if(Owner->bIsOnGround
			&& Owner->CurrentState != Jumping)
		{
			Jump();
			JumpGraceTimer = JumpGracePeriod;
		}
	}
}

void URJumpingComponent::SetJumpVelocity(float DeltaTime)
{
	float start = JumpGraph->GetFloatValue((JumpTimer - DeltaTime) / JumpDuration) * JumpHeight;
	float end = JumpGraph->GetFloatValue(JumpTimer / JumpDuration) * JumpHeight;
	
	float acceleration = (end - start)/DeltaTime;

	Owner->Velocity.Z = acceleration; 
}

void URJumpingComponent::Jump()
{
	if(Owner->bIsDead)
		return;
	
	if(!Owner->bIsOnGround
		|| Owner->CurrentState == Jumping)
	{
		JumpGraceTimer = 0.0f;
		return;
	}

	JumpTimer = 0;

	Owner->bIsOnGround = false;
	Owner->CurrentState = Jumping;

	OnJumpStart.Broadcast();
}

float URJumpingComponent::GetJumpPercent()
{
	if(JumpTimer >= JumpDuration)
		return 0.0f;

	return JumpTimer/JumpDuration;
}
