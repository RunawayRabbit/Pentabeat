// Yeet <3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RShockwave.generated.h"

class ARShockwave;

USTRUCT(BlueprintType)
struct FRShockwaveSpawnData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	TSubclassOf<ARShockwave> ShockwaveBPClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	float ZoneWidth = .1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Height = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnZOffset = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale = 4.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRFireShockwave, FRShockwaveSpawnData, ShockwaveSpawnData);

UCLASS()
class GP4_TEAM7_API ARShockwave : public AActor
{
	GENERATED_BODY()

public:
	ARShockwave();

	void FireShockwave(const FRShockwaveSpawnData& SpawnData);

protected:	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FRFireShockwave FireShockwaveEvent;
};
