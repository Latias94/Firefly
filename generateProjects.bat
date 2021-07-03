echo F|xcopy /S /Y /F dependence-premakes\GLFW\premake5.lua Firefly\vendor\GLFW\premake5.lua
call vendor\premake\premake5.exe vs2019
PAUSE