#pragma once

#include "CoreMinimal.h"

#include "RPellet.generated.h"

class URBeatReaction;

UCLASS()
class GP4_TEAM7_API ARPellet : public AActor
{
	GENERATED_BODY()

public:
	ARPellet();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** NOTE: Overridden version that rounds InLifespan up to the next full measure.*/
	virtual void SetLifeSpan(float InLifespan) override;

	UPROPERTY();
	UStaticMeshComponent* StaticMeshComponent;

	/** How much to scale the pellets when they flash. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleFactor;

	protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	URBeatReaction* BeatReaction;

	UPROPERTY();
	FVector BaseScale;

	/** How many *beats before death* do we start flashing on the 1st beat of a measure? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float StartFlashingOnes = 16.0f;
	/** How many *beats before death* do we start flashing on the 2nd beat of a measure? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float StartFlashingTwos = 4.0f;
	/** How many *beats before death* do we start flashing on the 3rd beat of a measure? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float StartFlashingThrees = 8.0f;
	/** How many *beats before death* do we start flashing on the 4th beat of a measure? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float StartFlashingFours = 4.0f;
};


