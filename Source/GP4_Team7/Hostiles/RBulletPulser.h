// Yeet <3

#pragma once

#include "CoreMinimal.h"

#include "RBullet.h"
#include "GP4_Team7/Midi/RBeatReaction.h"

#include "RBulletPulser.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_TEAM7_API URBulletPulser : public URBeatReaction
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URBulletPulser();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void RegisterMe(ARBullet* ARBullet);

	/** 1 = pulse every beat. 2 = every measure. In-between values lead to hard jumping on new measures, which also looks cool. */
	UPROPERTY(EditDefaultsOnly)
	float PulseTime = 2.0f;
	private:
	TArray<ARBullet*> Bullets;
};
