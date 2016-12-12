// Some copyright should be here...

using UnrealBuildTool;

public class SceneCaptureCubeHDRRenderPlugin : ModuleRules
{
	public SceneCaptureCubeHDRRenderPlugin(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"SceneCaptureCubeHDRRenderPlugin/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"SceneCaptureCubeHDRRenderPlugin/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
				"Engine",
                "InputCore",
				// ... add other public dependencies that you statically link with here ...
			}
			);


        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
