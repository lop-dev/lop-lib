::
::根据*.proto文件，生成官方的代码文件
::目前支持 C++、C#、Lua 这几种语言
::

::开启变量延时扩展
setlocal enabledelayedexpansion

::设置可执行程序
SET C1_EXE=..\..\..\..\..\..\EXLib\protobuf\bin\protoc
SET C2_EXE=..\..\..\..\..\..\EXLib\csharp\Protobuf-net\bin\protogen
SET LUA_PLUGIN=..\..\..\..\..\..\EXLib\protobuf-lua\protoc-gen-lua.bat

::删除并创建所需文件夹
SET SrcPath=.\12_ProtobufSrc_Out
RD /q /s %SrcPath%
MD %SrcPath%\C#
MD %SrcPath%\C++
MD %SrcPath%\Lua

SET CPP_OUT_PATH=../12_ProtobufSrc_Out/C++
SET CSHARP_OUT_PATH=../12_ProtobufSrc_Out/C#/
SET LUA_OUT_PATH=../12_ProtobufSrc_Out/Lua/

CD 11_ProTableGen_Out

::%C1_EXE% --help
::%C2_EXE% -p:import
::PAUSE

::First step : Gen base proto--------------------------------------------------------------------------------
::%C1_EXE%    --cpp_out=%CPP_OUT_PATH% 	baseDef.proto
::%C2_EXE%    -i:baseDef.proto  			-o:%CSHARP_OUT_PATH%/baseDef.pb.cs
::%C1_EXE% 	--plugin=protoc-gen-lua=%LUA_PLUGIN% --lua_out=%LUA_OUT_PATH% baseDef.proto
::-----------------------------------------------------------------------------------------------------------

::产生C++、C#、Lua代码
FOR %%i IN (*.proto) DO (
ECHO %%i
SET var=%%i
%C1_EXE%    --cpp_out=%CPP_OUT_PATH%	%%i
%C2_EXE%    -i:%%i			-o:./%CSHARP_OUT_PATH%/!var:.proto=.pb.cs!
%C1_EXE%    --plugin=protoc-gen-lua=%LUA_PLUGIN% --lua_out=%LUA_OUT_PATH% %%i
)

ECHO END

PAUSE