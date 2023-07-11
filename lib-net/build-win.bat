@echo off
setlocal EnableDelayedExpansion

set root_path=%~dp0
cd %root_path%

:: vcpkg (vcpkg.json)
set vcpkg_path=%root_path%vcpkg
set vcpkg_exe=%vcpkg_path%\vcpkg.exe
echo vcpkg_path: %vcpkg_path%
echo vcpkg_exe: %vcpkg_exe%
if not exist %vcpkg_exe% (
  set vcpkg_bat=%vcpkg_path%\bootstrap-vcpkg.bat
  echo vcpkg_bat: !vcpkg_bat!
  call !vcpkg_bat!
)
set VCPKG_DEFAULT_TRIPLET=x64-windows-static
:: %vcpkg_exe% install --triplet x64-windows-static

:: cmake (Debug/Release)
set config=Release
set install_path=install\%config%
set vcpkg_cmake=%vcpkg_path%\scripts\buildsystems\vcpkg.cmake
echo config: %config%
echo install_path: %install_path%
echo vcpkg_cmake: %vcpkg_cmake%
echo "cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%vcpkg_cmake% -DCMAKE_INSTALL_PREFIX=%install_path%"
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%vcpkg_cmake% -DCMAKE_INSTALL_PREFIX=%install_path%
echo "cmake --build build --target install --config %config%"
cmake --build build --target install --config %config%

:: run (main & test)
set vcpkg_installed=build\vcpkg_installed\x64-windows
if "%config%"=="Debug" (
  set vcpkg_installed=%vcpkg_installed%\debug
)
echo vcpkg_installed: %vcpkg_installed%

echo "copy %vcpkg_installed%\bin\*.dll %install_path%\bin"
copy %vcpkg_installed%\bin\*.dll %install_path%\bin

echo "%install_path%\bin\Main.exe"
%install_path%\bin\Main.exe
