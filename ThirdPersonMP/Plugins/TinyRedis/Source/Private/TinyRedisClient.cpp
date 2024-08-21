﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "TinyRedisClient.h"
#include "TinyRedisModule.h"

UTinyRedisClient* UTinyRedisClient::CreateRedisClient(const FString& InIP, int InPort, const FString& InPassword, int PoolSize)
{
	ITinyRedisModule* Module = ITinyRedisModule::Get();
	if (!Module)
	{
		return nullptr;
	}

	IRedisInterfacePtr Redis = Module->CreateRedisInterface(InIP, InPort, InPassword, PoolSize);
	if (!Redis)
	{
		return nullptr;
	}

	UTinyRedisClient* NewClient = NewObject<UTinyRedisClient>();
	NewClient->Redis = Redis;
	return NewClient;
}

FRedisReply UTinyRedisClient::ExecCommand(const FString& InCommand, ERedisCommandType InCommandTyp)
{
	FRedisReply Reply;
	if (Redis)
	{
		Reply = Redis->ExecCommand(InCommand, InCommandTyp);
	}
	return Reply;
}

TFuture<FRedisReply> UTinyRedisClient::AsyncExecCommand(const FString& InCommand, ERedisCommandType InCommandTyp)
{
	if (Redis)
	{
		return Redis->AsyncExecCommand(InCommand, InCommandTyp);
	}

	TPromise<FRedisReply> Promise;
	TFuture<FRedisReply> Future = Promise.GetFuture();
	return MoveTemp(Future);
}

void UTinyRedisClient::DumpRedisReply(const FRedisReply& Reply)
{
	UE_LOG(LogRedis, Warning, TEXT("%s"), *Reply.ToDebugString());
}