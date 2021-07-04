echo F|xcopy /S /Y /F dependence-premakes\GLFW\premake5.lua Firefly\vendor\GLFW\premake5.lua
echo F|xcopy /S /Y /F dependence-premakes\imgui\premake5.lua Firefly\vendor\imgui\premake5.lua
echo F|xcopy /S /Y /F dependence-premakes\imgui\CMakeLists.txt Firefly\vendor\imgui\CMakeLists.txt
call vendor\premake\premake5.exe vs2019
PAUSE