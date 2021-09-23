#include "GP4_Team7/Player/RGravityComponent.h"
#include "RPlayerPawn.h"

URGravityComponent::URGravityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URGravityComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ARPlayerPawn>(GetOwner());
}

void URGravityComponent::ApplyGravity(float DeltaTime)
{
	Owner->Velocity += (FVector::DownVector) * Gravity * DeltaTime;
}

void URGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!Owner->bIsOnGround
		&& Owner->CurrentState != Jumping)
	{
		ApplyGravity(DeltaTime);
	}
}

