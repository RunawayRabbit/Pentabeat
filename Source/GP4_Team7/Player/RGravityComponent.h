#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RGravityComponent.generated.h"

class ARPlayerPawn;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API URGravityComponent : public UActorComponent
{
	GENERATED_BODY()

	//Methods:
public:	
	URGravityComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
	void ApplyGravity(float DeltaTime);
	
	//Variables:
public:	

	UPROPERTY(EditAnywhere)
	float Gravity = 980.0f;
protected:

	UPROPERTY()
	ARPlayerPawn* Owner;
};
