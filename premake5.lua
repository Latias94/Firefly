staticRuntime = "off" -- VS only. on -> MultiThreaded, off -> MultiThreadedDLL.

workspace "Firefly"
    architecture "x86_64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["spdlog"]   = "%{wks.location}/Firefly/vendor/spdlog/include"
includeDir["GLFW"]     = "%{wks.location}/Firefly/vendor/GLFW/include"
includeDir["glad"]     = "%{wks.location}/Firefly/vendor/glad/include"
--includeDir["imgui"]    = "%{wks.location}/Firefly/vendor/imgui"
--includeDir["glm"]      = "%{wks.location}/Firefly/vendor/glm"
--includeDir["stb"]      = "%{wks.location}/Firefly/vendor/stb"
--includeDir["entt"]     = "%{wks.location}/Firefly/vendor/entt/include"
--includeDir["box2d"]    = "%{wks.location}/Firefly/vendor/box2d/include"
--includeDir["json"]     = "%{wks.location}/Firefly/vendor/json/include"
--includeDir["fmt"]      = "%{wks.location}/Firefly/vendor/fmt/include"
--includeDir["yaml_cpp"] = "%{wks.location}/Firefly/vendor/yaml-cpp/include"


clientIncludes = {
    "%{wks.location}/Firefly/src",
    "%{includeDir.spdlog}",
}

linkLibs = {
    "GLFW",
    "glad",
    --"imgui",
    --"box2d",
    --"yaml-cpp",
}

group "Dependencies"
    include "Firefly/vendor/GLFW"
    include "Firefly/vendor/glad"
    --include "Firefly/vendor/imgui"
    --include "Firefly/vendor/box2d"
    --include "Firefly/vendor/yaml-cpp"
group ""


include "Firefly"
include "Sandbox"


