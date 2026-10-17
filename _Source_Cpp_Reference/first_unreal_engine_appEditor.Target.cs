using UnrealBuildTool;
using System.Collections.Generic;

public class first_unreal_engine_appEditorTarget : TargetRules
{
	public first_unreal_engine_appEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("first_unreal_engine_app");
	}
}
