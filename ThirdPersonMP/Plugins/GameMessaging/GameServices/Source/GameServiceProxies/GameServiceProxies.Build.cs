// Copyright Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class GameServiceProxies : ModuleRules
	{
		public GameServiceProxies(ReadOnlyTargetRules Target) : base(Target)
		{
			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					"Messaging",
					"MessagingRpc",
				}
			);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
				}
			);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"CoreUObject",
					"GameMessaging",
					"GameServiceMessages",
				}
			);

			PrivateIncludePathModuleNames.AddRange(
				new string[]
				{
					"Messaging",
					"MessagingRpc",
					"GameServices",
				}
			);

			PrivateIncludePaths.AddRange(
				new string[]
				{
				}
			);
		}
	}
}