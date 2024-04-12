// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Misc/Guid.h"
#include "GameServiceRpcMessages.generated.h"


USTRUCT()
struct FGameServiceRpcLocateServer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category="Message")
	FString ServiceKey;
	
	/** The product's unique identifier. */
	UPROPERTY(EditAnywhere, Category="Message")
	FGuid ProductId;

	/** The product's version string. */
	UPROPERTY(EditAnywhere, Category="Message")
	FString ProductVersion;

	/** The mac address of the host. */
	UPROPERTY(EditAnywhere, Category = "Message")
	FString HostMacAddress;

	/** The user identification for the host. */
	UPROPERTY(EditAnywhere, Category = "Message")
	FString HostUserId;

	/** Default constructor. */
	FGameServiceRpcLocateServer() { }

	/** Create and initialize a new instance. */
	FGameServiceRpcLocateServer(const FGuid& InProductId, const FString& InProductVersion, const FString& InHostMacAddress, const FString& InHostUserId)
		: ProductId(InProductId)
		, ProductVersion(InProductVersion)
		, HostMacAddress(InHostMacAddress)
		, HostUserId(InHostUserId)
	{ }
};


USTRUCT()
struct FGameServiceRpcServer
{
	GENERATED_USTRUCT_BODY()

	/** The RPC server's message address as a string. */
	UPROPERTY(EditAnywhere, Category="Message")
	FString ServerAddress;

	/** Default constructor. */
	FGameServiceRpcServer() { }

	/** Create and initialize a new instance. */
	FGameServiceRpcServer(const FString& InServerAddress)
		: ServerAddress(InServerAddress)
	{ }
};
