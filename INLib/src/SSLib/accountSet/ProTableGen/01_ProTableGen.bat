::
::根据程序的*.xml配置文件，生成*.proto文件
::

::设置可执行程序
SET C1_EXE=.\01_ProTableGen\ProTableGen.exe

set ExtPath=.\11_ProTableGen_Out

::删除并创建所需文件夹
RD /q /s %ExtPath%
MD %ExtPath%\C#
MD %ExtPath%\C++
MD %ExtPath%\GO


XCOPY .\10_ProtobufDef\*.proto .\11_ProTableGen_Out

:: AccountServer 
for /f "delims=" %%i in ('dir /b "..\ProTable\AccountServer\*.xlsx"') do (
%C1_EXE%   ..\ProTable\AccountServer\%%i  ./11_ProTableGen_Out/%%~ni.proto
)

PAUSE