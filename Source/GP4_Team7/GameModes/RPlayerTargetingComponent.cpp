#include "GP4_Team7/GameModes/RPlayerTargetingComponent.h"

URPlayerTargetingComponent::URPlayerTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


AActor* URPlayerTargetingComponent::GetCurrentTarget() const
{
	return PlayerTarget;
};

bool URPlayerTargetingComponent::IsTargeting() const
{
	return PlayerTarget != nullptr;
}

void URPlayerTargetingComponent::SetTarget(AActor* NewTarget)
{
	PlayerTarget = NewTarget;
}
