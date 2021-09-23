#pragma once

#include "UObject/Interface.h"
#include "PlayerCharacterInterface.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPlayerCharacterInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IPlayerCharacterInterface
{
	GENERATED_IINTERFACE_BODY()

	/** Indicate to the camera whether or not we received input this frame */
	virtual bool GotMovementInput() = 0;
};

