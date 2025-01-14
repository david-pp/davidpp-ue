// Copyright 2020 Dan Kestranek.


#include "GDBlueprintLibrary.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "FileHelpers.h"
#include "GameplayTagsEditorModule.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Characters/Abilities/GDAbilitySystemComponent.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/CompilerResultsLog.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Tests/AutomationEditorCommon.h"

FGameplayAbilityTargetDataHandle UGDBlueprintLibrary::MakeTargetDataHandleByLocationInfo(const FGameplayAbilityTargetingLocationInfo& Source, const FGameplayAbilityTargetingLocationInfo& Target)
{
	FGameplayAbilityTargetDataHandle TargetDataHandle;

	FGameplayAbilityTargetData_LocationInfo* LocationInfo = new FGameplayAbilityTargetData_LocationInfo;
	LocationInfo->SourceLocation = Source;
	LocationInfo->TargetLocation = Target;
	TargetDataHandle.Add(LocationInfo);
	return TargetDataHandle;
}

FGameplayAbilityTargetDataHandle UGDBlueprintLibrary::MakeCustomTargetDataHandle(float Valule)
{
	FGameplayAbilityTargetDataHandle TargetDataHandle;

	FGameplayAbilityTargetData_Custom* Custom = new FGameplayAbilityTargetData_Custom;
	Custom->Value = Valule;

	TargetDataHandle.Add(Custom);
	return TargetDataHandle;
}

float UGDBlueprintLibrary::GetCustomTargetValueFromTargetData(const FGameplayAbilityTargetDataHandle& TargetData)
{
	int Index = 0;
	if (TargetData.Data.IsValidIndex(Index))
	{
		FGameplayAbilityTargetData* Data = TargetData.Data[Index].Get();

		FGameplayAbilityTargetData_Custom* Custom = static_cast<FGameplayAbilityTargetData_Custom*>(Data);
		if (Custom)
		{
			return Custom->Value;
		}
	}
	return 0.0f;
}

UGameplayEffect* UGDBlueprintLibrary::GenerateGameEffects(TSubclassOf<UGameplayEffect> Class, FName EffectName, FGameplayTag AssetTag, bool NeedSave)
{
	if (!Class) return nullptr;

	UClass* ParentClass = Class.Get();
	FString PackageName = FString(TEXT("/Game/GeneratedGEs")) / EffectName.ToString();
	UPackage* Package = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("GenerateGameEffects - %s"), *PackageName);

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *PackageName, nullptr, LOAD_NoWarn | LOAD_NoRedirects);
	if (Blueprint)
	{
		// Update
		Package = Cast<UPackage>(Blueprint->GetOuter());
		if (Blueprint->ParentClass != ParentClass)
		{
			Blueprint->ParentClass = ParentClass;
		}
	}
	else
	{
		// Create New
		UClass* BlueprintClass = UBlueprint::StaticClass();
		UClass* BlueprintGeneratedClass = UBlueprintGeneratedClass::StaticClass();
		Package = CreatePackage(*PackageName);
		if (Package)
		{
			EBlueprintType BlueprintType = ParentClass && ParentClass->IsChildOf(UBlueprintFunctionLibrary::StaticClass()) ? BPTYPE_FunctionLibrary : BPTYPE_Normal;

			// Create and init a new Blueprint
			Blueprint = FKismetEditorUtilities::CreateBlueprint(ParentClass, Package, EffectName, BlueprintType, BlueprintClass, BlueprintGeneratedClass);

			if (Blueprint)
			{
				// Notify the asset registry
				FAssetRegistryModule::AssetCreated(Blueprint);
			}
		}
	}

	// To Set CDO
	UGameplayEffect* GameEffectCDO = nullptr;
	if (Blueprint)
	{
		// Editing the BluePrint
		GameEffectCDO = Blueprint->GeneratedClass.Get()->GetDefaultObject<UGameplayEffect>();
		if (GameEffectCDO)
		{
			GameEffectCDO->InheritableGameplayEffectTags = FInheritedTagContainer();
			if (AssetTag.IsValid())
			{
				GameEffectCDO->InheritableGameplayEffectTags.Added.AddTag(AssetTag);
			}
		}
	}

	// Compile & Save
	if (Blueprint && Package && NeedSave)
	{
		// Mark the package dirty...
		Package->MarkPackageDirty();

		// Compile
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
		FKismetEditorUtilities::CompileBlueprint(Blueprint);

		// Save
		TArray<UPackage*> PackagesToSave;
		PackagesToSave.Add(Package);
		FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);
	}

	return GameEffectCDO;
}

bool UGDBlueprintLibrary::DeleteGameEffectAsset(FName EffectName)
{
	FString PackageName = FString(TEXT("/Game/GeneratedGEs")) / EffectName.ToString();
	UPackage* Package = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("DeleteGameEffectAsset - %s"), *PackageName);

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *PackageName, nullptr, LOAD_NoWarn | LOAD_NoRedirects);
	if (Blueprint)
	{
		Package = Cast<UPackage>(Blueprint->GetOuter());

		TArray<UObject*> ObjectsToForceDelete;
		ObjectsToForceDelete.Add(Blueprint);

		int32 ObjectsDeleted = ObjectTools::ForceDeleteObjects(ObjectsToForceDelete, false);
		return ObjectsDeleted > 0;

#if 0
		FAssetRegistryModule::AssetDeleted(Blueprint);
		
		bool bSuccessful = ObjectTools::DeleteSingleObject(Blueprint, false);

		//If we failed to delete this object manually clear any references and try again
		if (!bSuccessful)
		{
			//Clear references to the object so we can delete it
			FAutomationEditorCommonUtils::NullReferencesToObject(Blueprint);
			bSuccessful = ObjectTools::DeleteSingleObject(Blueprint, false);
		}
		
		return bSuccessful;

#endif
	}

	return false;
}

bool UGDBlueprintLibrary::SaveGameEffectAsset(FName EffectName)
{
	FString PackageName = FString(TEXT("/Game/GeneratedGEs")) / EffectName.ToString();
	UPackage* Package = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("SaveGameEffectAsset - %s"), *PackageName);

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *PackageName, nullptr, LOAD_NoWarn | LOAD_NoRedirects);
	if (Blueprint)
	{
		// Mark the package dirty...
		Package = Cast<UPackage>(Blueprint->GetOuter());
		Package->MarkPackageDirty();

		// Compile
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
		FKismetEditorUtilities::CompileBlueprint(Blueprint);

		// Save
		TArray<UPackage*> PackagesToSave;
		PackagesToSave.Add(Package);
		FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);

		return true;
	}

	return false;
}

void UGDBlueprintLibrary::AddGameTagRuntime(FString TagName, FString Comment, FName TagSourceName)
{
	IGameplayTagsEditorModule::Get().AddNewGameplayTagToINI(TagName, Comment, TagSourceName);
}
