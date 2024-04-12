// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IMessageRpcClient.h"
#include "Misc/TypeContainer.h"
#include "IGameServiceProvider.h"
#include "User/IGameUserService.h"
#include "User/GameUserProxy.h"

/**
 * Implements the GameServiceProxies module.
 */
class FGameServiceProxiesModule : public IGameServiceProvider
{
public:
	// IModuleInterface interface

	virtual void StartupModule() override
	{
		// GameServiceUser_SupportedServiceNames.Add(TNameOf<IGameUserService>::GetName());
		// GameServiceUserLogin_SupportedServiceNames.Add(TNameOf<IGameServiceUserLogin>::GetName());
	}

	virtual void ShutdownModule() override
	{
	}

	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

public:
	// IGameServiceProvider interface
	virtual TSharedPtr<IGameService> GetService(const FString& ServiceName, const TSharedRef<FTypeContainer>& Dependencies) override
	{
		// TSharedPtr<IMessageRpcClient> RpcClient = Dependencies->GetInstance<IMessageRpcClient>();
		//
		// if (!RpcClient.IsValid())
		// {
		// 	return nullptr;
		// }

		if (ServiceName == TEXT("GameUserProxy"))
		{
			return FGameUserProxyFactory::Create();
		}

		//Add additional supported proxy services here

		return nullptr;
	}

private:
	TSet<FString> GameServiceUser_SupportedServiceNames;
	// TSet<FString> GameServiceUserLogin_SupportedServiceNames;
};


IMPLEMENT_MODULE(FGameServiceProxiesModule, GameServiceProxies);