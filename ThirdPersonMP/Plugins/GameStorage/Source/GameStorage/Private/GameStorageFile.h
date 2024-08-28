﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStorageEngine.h"
#include "GameStoragePath.h"
#include "GameStorageSerializer.h"

/**
 * File System Backend
 */
class GAMESTORAGE_API FGameStorageFile : public IGameStorageEngine
{
public:
	FGameStorageFile(const FGameStorageEngineSettings& InSettings);
	virtual ~FGameStorageFile() override;

	FString GetRootDir() const;
	FString MakeFilePath(const FGameStoragePath& StoragePath) const;
	FString MakeFilePath(const FGameStorageKey& StoragePath) const;

protected:
	bool SaveObjectToFile(UObject* Object, const FString& FilePath);
	bool LoadObjectFromFile(UObject* Object, const FString& FilePath);

public:
	// ~ Impl IGameStorageEngine Begin
	virtual FString GetNamespace() const override;

	virtual bool SaveObject(UObject* Object, const FString& Path) override;
	virtual bool LoadObject(UObject* Object, const FString& Path) override;
	virtual bool LoadObjects(TArray<UObject*>& Objects, TSubclassOf<UObject> Class, const FString& PathPattern, UObject* Outer) override;
	virtual bool DeleteObject(const FString& Path) override;

	virtual bool AsyncSaveEntity(IGameStorageEntityPtr Entity, const FNativeOnStorageEntitySaveDelegate& OnSave) override;
	virtual bool AsyncLoadEntity(IGameStorageEntityPtr Entity, const FNativeOnStorageEntityLoadDelegate& OnLoad) override;
	// ~ Impl IGameStorageEngine End

protected:
	FGameStorageEngineSettings Settings;
	IGameStorageSerializerPtr Serializer;
};
