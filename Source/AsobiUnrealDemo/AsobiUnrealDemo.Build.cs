using UnrealBuildTool;

public class AsobiUnrealDemo : ModuleRules
{
	public AsobiUnrealDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"Slate",
			"SlateCore",
			"AsobiSDK"
		});
	}
}
