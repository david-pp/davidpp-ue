// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class OnlineSubsystemRPG : ModuleRules
{
	public OnlineSubsystemRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDefinitions.Add("ONLINESUBSYSTEMNULL_PACKAGE=1");
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"OnlineSubsystemUtils", 
				"JsonUtilities", 
				"DSMaster"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Sockets",
				"OnlineSubsystem",
				"Json",
				"JsonUtilities",
				"HTTP",
				"HTTPServer", 
				"DSMaster"
			}
		);
	}
}