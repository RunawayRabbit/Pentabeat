// Yeet <3

#include "RShockwave.h"

ARShockwave::ARShockwave()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARShockwave::FireShockwave(const FRShockwaveSpawnData& SpawnData)
{
	FireShockwaveEvent.Broadcast(SpawnData);
}
