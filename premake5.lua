workspace "Engine"
    location "build"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    project "Lib"
        location "build/Lib"
        kind "StaticLib"
        language "C++"
        cppdialect "C++latest"

        files
        {
            "Lib/inc/**.h",
            "Lib/public/**.h",
            "Lib/src/**.cpp"
        }

        includedirs
        {
            "Lib/inc",
            "Lib/public"
        }

        defines
        {
            "NGN_EXPORTS"
        }

        filter "system:windows"
            files { "Lib/win32/**.cpp" }
            links { "d3d11", "d3dcompiler", "dxguid", "dxgi" }

        filter "configurations:Debug"
            defines { "NGN_DEBUG" }
            symbols "On"

        filter "configurations:Release"
            optimize "On"

        filter "configurations:Dist"
            optimize "On"


    project "Sandbox"
        location "build/Sandbox"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++latest"
        debugdir "Working Directory"

        files
        {
            "Sandbox/inc/**.h",
            "Sandbox/src/**.cpp"
        }

        includedirs
        {
            "Sandbox/inc",
            "Lib/public"
        }

        links
        {
            "Lib"
        }

        filter "configurations:Debug"
            defines { "SANDBOX_DEBUG" }
            symbols "On"

        filter "configurations:Release"
            optimize "On"

        filter "configurations:Dist"
            optimize "On"

        filter "system:windows"
            entrypoint "WinMainCRTStartup"

    project "Editor"
        location "build/Editor"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++latest"
        debugdir "Working Directory"

        files
        {
            "Editor/inc/**.h",
            "Editor/src/**.cpp"
        }

        includedirs
        {
            "Editor/inc",
            "Lib/public"
        }

        links
        {
            "Lib"
        }

        filter "configurations:Debug"
            defines { "EDITOR_DEBUG" }
            symbols "On"

        filter "configurations:Release"
            optimize "On"

        filter "configurations:Dist"
            optimize "On"

        filter "system:windows"
            entrypoint "WinMainCRTStartup"

    project "AssetBundler"
        location "build/AssetBundler"
        kind "WindowedApp"
        language "C#"
        flags { "WPF" }
        libdirs { "../AssetBundler/Lib" }

        files
		{
			"AssetBundler/**.cs",
			"AssetBundler/**.xaml",
			"AssetBundler/**.xaml.cs"
		}

        links
		{
            "WindowsBase",
			"Microsoft.Csharp",
            "PresentationCore",
            "PresentationFramework",
            "System",
            "System.Core",
            "System.Data",
            "System.Data.DataSetExtensions",
            "System.Xml",
            "System.Xml.Linq",
            "System.Xaml",
		}

        nuget
        {
            "SharpDX.D3DCompiler:4.2.0"
        }

        filter "files:AssetBundler/App.xaml"
            buildaction "Embed"

        filter "files:AssetBundler/Application.xaml"
            buildaction "Application"

		filter "configurations:Debug"
			symbols "On"

		filter "configurations:Release"
			optimize "On"

		filter "configurations:Dist"
			optimize "On"
