// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionSystemModule.h"
#include "Modules/ModuleManager.h"

void FActionSystemModule::StartupModule()
{
	// Put your module initialization code here
}

void FActionSystemModule::ShutdownModule()
{
	// Put your module termination code here
}

IMPLEMENT_GAME_MODULE(FActionSystemModule, ActionSystem);
DEFINE_LOG_CATEGORY(LogActionSystem);