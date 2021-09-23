// Yeet <3

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RBulletPulsingHandlingComponent.generated.h"

class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM7_API URBulletPulsingHandlingComponent : public UActorComponent
{
	GENERATED_BODY()

	//Methods:
public:	
	URBulletPulsingHandlingComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void ExtrudeBullets();
	void GraduallyUnExtrudeBullets(float DeltaTime);
	void SetParameter();

	//Variables:
public:
	
protected:		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletUnExtrudeSpeed = 5.0f;

	UPROPERTY()
	float CurrentExtrudeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* BulletParameterCollection;

	UPROPERTY()
	UMaterialParameterCollectionInstance* BulletParameterInstance;
};
