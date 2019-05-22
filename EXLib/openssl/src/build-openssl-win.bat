@echo off

setlocal

if "%VisualStudioVersion%" equ "" echo Please run this script from a Visual Studio command prompt for the compiler you want to use
if "%VisualStudioVersion%" equ "" exit /b 1

:: make perl.exe available via %PATH%
set PATH=%PATH%;

pushd %~dp0src

perl Configure VC-WIN32 no-asm --prefix=%~dp0\win32-mt
call ms\do_ms.bat
nmake -f ms\nt.mak
nmake -f ms\nt.mak install
copy /y tmp32\lib.pdb %~dp0\win32-mt\lib\
nmake -f ms\nt.mak clean

perl Configure debug-VC-WIN64A no-asm --prefix=%~dp0\win32-mtd
call ms\do_ms.bat
nmake -f ms\nt.mak
nmake -f ms\nt.mak install
copy /y tmp32.dbg\lib.pdb %~dp0\win32-mtd\lib\
nmake -f ms\nt.mak clean

::
:: I'm sorry, but this is honestly the best answer @gorlak
::

copy /y ms\nt.mak ms\nt.mak.orig

perl Configure VC-WIN32 no-asm --prefix=%~dp0\win32-md
call ms\do_ms.bat
copy /y ms\nt.mak ms\nt.mak.unhacked
perl -p -e "s/\/MT/\/MD/g" ms\nt.mak.unhacked > ms\nt.mak
nmake -f ms\nt.mak
nmake -f ms\nt.mak install
copy /y tmp32\lib.pdb %~dp0\win32-md\lib\
nmake -f ms\nt.mak clean

perl Configure debug-VC-WIN32 no-asm --prefix=%~dp0\win32-mdd
call ms\do_ms.bat
copy /y ms\nt.mak ms\nt.mak.unhacked
perl -p -e "s/\/MT/\/MD/g" ms\nt.mak.unhacked > ms\nt.mak
nmake -f ms\nt.mak
nmake -f ms\nt.mak install
copy /y tmp32.dbg\lib.pdb %~dp0\win32-mdd\lib\
nmake -f ms\nt.mak clean

copy /y ms\nt.mak.orig ms\nt.mak

popd

pushd %~dp0

if exist install attrib -r install\*.* /s
if exist install rmdir /s /q install

ren win32-mt\lib mt
ren win32-mtd\lib mtd
ren win32-md\lib md
ren win32-mdd\lib mdd

ren win32-mt install
mkdir install\lib\vstudio-%VisualStudioVersion%\win32
move install\mt install\lib\vstudio-%VisualStudioVersion%\win32\
move win32-mtd\mtd install\lib\vstudio-%VisualStudioVersion%\win32\
move win32-md\md install\lib\vstudio-%VisualStudioVersion%\win32\
move win32-mdd\mdd install\lib\vstudio-%VisualStudioVersion%\win32\

rmdir /s /q win32-mtd
rmdir /s /q win32-md
rmdir /s /q win32-mdd

popd

endlocal