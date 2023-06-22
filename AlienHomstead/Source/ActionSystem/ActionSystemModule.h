#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FActionSystemModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

DECLARE_LOG_CATEGORY_EXTERN(LogActionSystem, Log, All);