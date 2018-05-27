// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class FPSGame : ModuleRules
{
	public FPSGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        LoadSqlite3Library(Target);
    }

    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../3rdParty/")); }
    }

    public bool LoadSqlite3Library(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "sqlite3", "Libraries");

            // test your path with:
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "sqlite3." + PlatformString + ".lib"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "sqlite3", "Includes"));
        }

        PublicDefinitions.Add(string.Format("WITH_SQLITE3_BINDING={0}", isLibrarySupported ? 1 : 0));
        
        return isLibrarySupported;
    }
}
