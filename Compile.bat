set OF_ROOT=/d/mygit/openFrameworks
set OF_ADDONS_PATH=/d/mygit/openFrameworks/addons

set WHERE=%CD%

D:\msys64\msys2_shell.cmd -mingw32 -where %WHERE%

















































:loop
open Compile.bat
goto loop