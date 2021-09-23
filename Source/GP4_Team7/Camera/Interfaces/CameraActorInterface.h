#pragma once

#include "UObject/Interface.h"
#include "CameraActorInterface.generated.h"

class ARCameraModificationVolume;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCameraActorInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ICameraActorInterface
{
	GENERATED_IINTERFACE_BODY()

	/** Used to indicate to the camera when we enter or exit camera modification volumes */
    virtual ARCameraModificationVolume* GetCurrentCameraModificationVolume() const = 0;

	/** Used to indicate to the camera when we enter or exit camera modification volumes */
	virtual void SetCurrentCameraModificationVolume(ARCameraModificationVolume* CurrentCameraModificationVolume) = 0;
};
