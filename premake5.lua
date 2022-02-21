workspace "GTDLi"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GTDLi"
    location "GTDLi"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "PCH.h"
    pchsource "GTDLi/include/PCH/PCH.cpp"

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/include/**.h",
        "%{prj.name}/include/PCH/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor",
        "%{prj.name}/include"
    }

    links
    {
        "User32.lib",
        "gdi32.lib",
        "shell32.lib",
        "Xinput.lib",
        "Winmm.lib",
        "bcrypt.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "GTD_PLATFORM_WINDOWS",
            "GTD_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "GTD_DEBUG"
        symbols "On"    
        
    filter "configurations:Release"
        defines "GTD_RELEASE"
        optimize "On"    
        
    filter "configurations:Debug"
        defines "GTD_DIST"
        optimize "On"



 project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/include/**.h"
    }

    includedirs
    {
        "%{prj.name}/vendor",
        "GTDLi/src",
        "GTDLi/include"
    }

    links
    {
        "GTDLi"
    }

    
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "GTD_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "GTD_DEBUG"
        symbols "On"    
        
    filter "configurations:Release"
        defines "GTD_RELEASE"
        optimize "On"    
        
    filter "configurations:Debug"
        defines "GTD_DIST"
        optimize "On"
