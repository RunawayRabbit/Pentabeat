#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "RJumpingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpDelegate);

class ARPlayerPawn;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API URJumpingComponent : public UActorComponent
{
	GENERATED_BODY()

	//Methods:
public:	
	URJumpingComponent();

	void Jump();

	UFUNCTION(BlueprintCallable)
	float GetJumpPercent();
protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetJumpVelocity(float DeltaTime);
	
	//Variables:
public:	
	UPROPERTY(EditAnywhere)
	float JumpHeight = 1000.0f;

	UPROPERTY(EditAnywhere)
	float JumpDuration = 1.0f;

	UPROPERTY(EditAnywhere)
	float JumpGracePeriod = 0.2f;
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* JumpGraph;

	UPROPERTY(BlueprintAssignable)
	FJumpDelegate OnJumpStart;

	UPROPERTY(BlueprintAssignable)
	FJumpDelegate OnJumpLand;

protected:
	UPROPERTY()
	ARPlayerPawn* Owner;

	UPROPERTY()
	float JumpTimer = 99999.0f;

	float JumpGraceTimer = 99999.0f;
};
