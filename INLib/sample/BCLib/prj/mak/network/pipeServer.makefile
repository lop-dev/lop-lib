################################################################
#公共参数
################################################################
MAKE_PATH=../make
include $(MAKE_PATH)/makecmd.inc

################################################################
#编译参数设置
################################################################
# 多个编译源文件目录的共同根目录
CPP_ROOT_DIR=../../..

# 多个编译源文件目录
CPP_SRC_DIR=../../../network/pipe/server

# 包含头文件目录
DEBUG_INCS=-I../../../../../inc -I../../../../../../EXLib
RELEASE_INCS=-I../../../../../inc -I../../../../../../EXLib

# 包含库文件目录
DEBUG_LIBS=-L../../../../../lib/BCLib/cos-x86-64
RELEASE_LIBS=-L../../../../../lib/BCLib/cos-x86-64

# 输出中间文件目录
DEBUG_OUT_DIR=../../../../../../BCSrcOut/sample/cos-x86-64/network/pipe/server/debug
RELEASE_OUT_DIR=../../../../../../BCSrcOut/sample/cos-x86-64/network/pipe/server/release

# 输出目标文件目录
OUT_TARGET_DIR=../../../build/network/pipe

# 额外的宏定义
DEBUG_DEFS=-DLOG4CXX_STATIC
RELEASE_DEFS=-DLOG4CXX_STATIC

# 编译C++文件参数
DEBUG_CXX_FLAGS=-Wno-deprecated
RELEASE_CXX_FLAGS=-Wno-deprecated

# 编译C文件参数
DEBUG_C_FLAGS=-Wno-deprecated
RELEASE_C_FLAGS=-Wno-deprecated

################################################################
#编译文件
################################################################
OBJ_CPP=$(call fun_get_cpp_src_file)
OBJ_C=
OBJ_S=

################################################################
#静态库参数设置
################################################################
# 生产静态库参数
DEBUG_LIB_FLAGS=
RELEASE_LIB_FLAGS=

# 输出静态库
DEBUG_LIB_FILE=
RELEASE_LIB_FILE=

################################################################
#动态库参数设置
################################################################
# 生产静态库参数
DEBUG_DLL_FLAGS=
RELEASE_DLL_FLAGS=

# 输出动态库
DEBUG_DLL_FILE=
RELEASE_DLL_FILE=

################################################################
#可执行文件参数设置
################################################################
# 生产静态库参数
DEBUG_EXE_FLAGS=-Wl,--start-group -lrt -pthread -lNetwork_d -lUtility_d -lFramework_d -Wl,--end-group -Wl,-rpath,./
RELEASE_EXE_FLAGS=-Wl,--start-group -lrt -pthread -lNetwork -lUtility -lFramework -Wl,--end-group -Wl,-rpath,./

# 输出静态库
DEBUG_EXE_FILE=$(OUT_TARGET_DIR)/pipeServer_d.out
RELEASE_EXE_FILE=$(OUT_TARGET_DIR)/pipeServer.out

include $(MAKE_PATH)/makeobj.inc
