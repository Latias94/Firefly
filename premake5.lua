workspace "Firefly"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "Firefly/vendor/spdlog/include",
        "Firefly/src"
    }

    links
    {
        "Firefly"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "FF_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "FF_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "FF_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "FF_DIST"
        optimize "On"
        
project "Firefly"
    location "Firefly"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "FF_BUILD_DLL",
            "FF_PLATFORM_WINDOWS",
            "_WINDLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "FF_DEBUG"
        symbols "On"
    filter "configurations:Release"
        defines "FF_RELEASE"
        optimize "On"
    filter "configurations:Dist"
        defines "FF_DIST"
        optimize "On"
