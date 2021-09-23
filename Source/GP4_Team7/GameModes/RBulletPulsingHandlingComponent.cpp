#include "GP4_Team7/GameModes/RBulletPulsingHandlingComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

URBulletPulsingHandlingComponent::URBulletPulsingHandlingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URBulletPulsingHandlingComponent::BeginPlay()
{
	BulletParameterInstance = GetWorld()->GetParameterCollectionInstance(BulletParameterCollection);
}

void URBulletPulsingHandlingComponent::ExtrudeBullets()
{
	CurrentExtrudeMultiplier = 1.0f;
	SetParameter();
}

void URBulletPulsingHandlingComponent::GraduallyUnExtrudeBullets(float DeltaTime)
{
	const float potentialDecrease = DeltaTime * BulletUnExtrudeSpeed;
	if(CurrentExtrudeMultiplier < 0 + potentialDecrease)
	{
		CurrentExtrudeMultiplier = 0.0f;
	}
	else
	{
		CurrentExtrudeMultiplier -= potentialDecrease;
	}
	SetParameter();
}

void URBulletPulsingHandlingComponent::SetParameter()
{
	BulletParameterInstance->SetScalarParameterValue("BulletExtrusionMultiplier", CurrentExtrudeMultiplier);
}


void URBulletPulsingHandlingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentExtrudeMultiplier > 0)
	{
		GraduallyUnExtrudeBullets(DeltaTime);
	}
}


