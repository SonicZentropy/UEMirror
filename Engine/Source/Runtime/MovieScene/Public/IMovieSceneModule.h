// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

/**
 * The public interface of the MovieScene module
 */
class IMovieSceneModule
	: public IModuleInterface
{
public:

	/**
	 * Singleton-like access to IMovieScene
	 *
	 * @return Returns MovieScene singleton instance, loading the module on demand if needed
	 */
	static inline IMovieSceneModule& Get()
	{
		return FModuleManager::LoadModuleChecked< IMovieSceneModule >( "MovieScene" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "MovieScene" );
	}
};
