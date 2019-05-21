################################################################
#这个文件中的内容是相对固定不变的，如要修改，请通知AARON统一处理
################################################################
ifndef fun_get_s_src_file
define fun_get_s_src_file
	$(foreach n, $(CPP_SRC_DIR), $(if $(n), $(shell find $(n) -name *.s)))
endef
endif

ifndef fun_get_c_src_file
define fun_get_c_src_file
	$(foreach n, $(CPP_SRC_DIR), $(if $(n), $(shell find $(n) -name *.c)))
endef
endif

ifndef fun_get_cpp_src_file
define fun_get_cpp_src_file
	$(foreach n, $(CPP_SRC_DIR), $(if $(n), $(shell find $(n) -name *.cpp)))
endef
endif

ifndef fun_get_cc_src_file
define fun_get_cc_src_file
	$(foreach n, $(CPP_SRC_DIR), $(if $(n), $(shell find $(n) -name *.cc)))
endef
endif

ifndef fun_get_pkg_src_file
define fun_get_pkg_src_file
	$(foreach n, $(CPP_SRC_DIR), $(if $(n), $(shell find $(n) -name *.pkg)))
endef
endif

#ifndef fun_get_cpp_src_file
#define fun_get_cpp_src_file
#	$(if $(CPP_SRC_DIR), $(wildcard $(CPP_SRC_DIR)/*.cpp), ) $(foreach n, $1, $(if $(CPP_SRC_DIR), $(wildcard $(CPP_SRC_DIR)/$(n)/*.cpp), )) 
#endef
#endif

MAKE_TARGET=$@

_DEBUG_INCS=-I.
_RELEASE_INCS=-I.

_DEBUG_DEFS=-D_DEBUG -D_LINUX
_RELEASE_DEFS=-D_LINUX

_DEBUG_LIBS=
_RELEASE_LIBS=

CMD_S=gcc
CMD_C=gcc
CMD_CXX=g++
CMD_DLL=g++
CMD_LIB=ar -rsv
CMD_EXE=g++

CMD_TOLUAPP=$(TOLUAPP_BIN_PATH)/tolua++

_DEBUG_C_FLAGS=-c -g -fPIC -Wall -finput-charset=GB2312 -fexec-charset=UTF-8 -std=c++11
_RELEASE_C_FLAGS=-c -O3 -fPIC -Wall -finput-charset=GB2312 -fexec-charset=UTF-8 -std=c++11

#_DEBUG_C_FLAGS=-c -fPIC -Wall -Wno-format -g
#_RELEASE_C_FLAGS=-c -fPIC -Wall -Wno-unknown-pragmas -Wno-format -O3

_DEBUG_CXX_FLAGS=$(_DEBUG_C_FLAGS)
_RELEASE_CXX_FLAGS=$(_RELEASE_C_FLAGS)

_DEBUG_DLL_FLAGS=-shared -g -fPIC -Wall
_RELEASE_DLL_FLAGS=-shared -O3 -fPIC -Wall

_DEBUG_EXE_FLAGS=-g
_RELEASE_EXE_FLAGS=
