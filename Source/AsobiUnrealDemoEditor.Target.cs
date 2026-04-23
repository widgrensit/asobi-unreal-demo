using UnrealBuildTool;
using System.Collections.Generic;

public class AsobiUnrealDemoEditorTarget : TargetRules
{
	public AsobiUnrealDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("AsobiUnrealDemo");
	}
}
