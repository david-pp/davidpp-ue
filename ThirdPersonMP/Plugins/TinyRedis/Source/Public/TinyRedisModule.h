#pragma once

#include "CoreMinimal.h"
#include "TinyRedisInterface.h"

/** Declares a log category for this module. */
DECLARE_LOG_CATEGORY_EXTERN(LogRedis, Log, All);

class ITinyRedisModule : public IModuleInterface
{
public:
	static ITinyRedisModule* Get()
	{
		return static_cast<ITinyRedisModule*>(FModuleManager::Get().LoadModule("TinyRedis"));
	}

	static IRedisInterfacePtr GetTinyRedis()
	{
		ITinyRedisModule* Module = Get();
		return Module ? Module->GetRedisInterface() : nullptr;
	}

public:
	/*
     * Get default redis interface
     */
	virtual IRedisInterfacePtr GetRedisInterface() const = 0;

	/**
	 * Create a new redis client impl
	 */
	virtual IRedisInterfacePtr CreateRedisInterface(const FString& InIP, int InPort, const FString& InPassword, int InPoolSize) const = 0;
};