using UnrealBuildTool;
using System.Collections.Generic;

public class first_unreal_engine_appTarget : TargetRules
{
	public first_unreal_engine_appTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("first_unreal_engine_app");
	}
}
