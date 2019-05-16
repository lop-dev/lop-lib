################################################################
#这个文件中的内容是相对固定不变的，如要修改，请通知AARON统一处理
################################################################
ifndef fun_clean_file
define fun_clean_file
	@for i in $1;\
	do\
		if [ -f $$i ]; then echo "rm -f "$$i";"; rm -f $$i; fi \
	done
endef
endif

ifndef fun_create_dir
define fun_create_dir
	@if [ ! -d $1 ]; then echo "mkdir -p "$1";"; mkdir -p $1; fi
endef
endif

ifeq (YES, $(PROFILE))
   PG=-pg
endif

ifndef DEBUG_OUT_DIR
	DEBUG_OUT_DIR=.
endif

ifndef RELEASE_OUT_DIR
	RELEASE_OUT_DIR=.
endif

ifndef CPP_SRC_DIR
	CPP_SRC_DIR=.
endif

DEBUG_LIB_FLAGS+=$(_DEBUG_LIB_FLAGS)
DEBUG_DLL_FLAGS+=$(_DEBUG_DLL_FLAGS)
DEBUG_EXE_FLAGS+=$(_DEBUG_EXE_FLAGS)
RELEASE_LIB_FLAGS+=$(_RELEASE_LIB_FLAGS)
RELEASE_DLL_FLAGS+=$(_RELEASE_DLL_FLAGS)
RELEASE_EXE_FLAGS+=$(_RELEASE_EXE_FLAGS)

DEBUG_C_FLAGS+=$(_DEBUG_C_FLAGS)
RELEASE_C_FLAGS+=$(_RELEASE_C_FLAGS)
DEBUG_CXX_FLAGS+=$(_DEBUG_CXX_FLAGS)
RELEASE_CXX_FLAGS+=$(_RELEASE_CXX_FLAGS)

DEBUG_LIBS+=$(_DEBUG_LIBS)
RELEASE_LIBS+=$(_RELEASE_LIBS)

DEBUG_INCS+=$(_DEBUG_INCS)
RELEASE_INCS+=$(_RELEASE_INCS)

DEBUG_DEFS+=$(_DEBUG_DEFS)
RELEASE_DEFS+=$(_RELEASE_DEFS)

TOLUA_H_PKG=$(CPP_PKG:%.pkg=%.tolua.h)
TOLUA_CPP_PKG=$(CPP_PKG:%.pkg=%.tolua.cc)

DEBUG_OBJ_TOLUA_CPP=$(subst $(CPP_ROOT_DIR), $(DEBUG_OUT_DIR), $(TOLUA_CPP_PKG:%.cc=%.d))
DEBUG_OBJ_CPP=$(subst $(CPP_ROOT_DIR), $(DEBUG_OUT_DIR), $(OBJ_CPP:%.cpp=%.d))
DEBUG_OBJ_C=$(subst $(CPP_ROOT_DIR), $(DEBUG_OUT_DIR), $(OBJ_C:%.c=%.d))
DEBUG_OBJ_S=$(subst $(CPP_ROOT_DIR), $(DEBUG_OUT_DIR), $(OBJ_S:%.s=%.d))
DEBUG_OBJ=$(DEBUG_OBJ_TOLUA_CPP) $(DEBUG_OBJ_CPP) $(DEBUG_OBJ_C) $(DEBUG_OBJ_S)

RELEASE_OBJ_TOLUA_CPP=$(subst $(CPP_ROOT_DIR), $(RELEASE_OUT_DIR), $(TOLUA_CPP_PKG:%.cc=%.d))
RELEASE_OBJ_CPP=$(subst $(CPP_ROOT_DIR), $(RELEASE_OUT_DIR), $(OBJ_CPP:%.cpp=%.d))
RELEASE_OBJ_C=$(subst $(CPP_ROOT_DIR), $(RELEASE_OUT_DIR), $(OBJ_C:%.c=%.d))
RELEASE_OBJ_S=$(subst $(CPP_ROOT_DIR), $(RELEASE_OUT_DIR), $(OBJ_S:%.s=%.d))
RELEASE_OBJ=$(RELEASE_OBJ_TOLUA_CPP) $(RELEASE_OBJ_CPP) $(RELEASE_OBJ_C) $(RELEASE_OBJ_S)

CLEAN_FILE+=$(DEBUG_OBJ)
CLEAN_FILE+=$(RELEASE_OBJ)

CLEAN_ALL_FILE += $(DEBUG_LIB_FILE) $(DEBUG_DLL_FILE) $(DEBUG_EXE_FILE) $(RELEASE_LIB_FILE) $(RELEASE_DLL_FILE) $(RELEASE_EXE_FILE) $(CLEAN_FILE) $(TOLUA_CPP_PKG) $(TOLUA_H_PKG)

debug: $(DEBUG_LIB_FILE) $(DEBUG_DLL_FILE) $(DEBUG_EXE_FILE)

release: $(RELEASE_LIB_FILE) $(RELEASE_DLL_FILE) $(RELEASE_EXE_FILE)

debuglib: $(DEBUG_LIB_FILE)

debugdll: $(DEBUG_DLL_FILE)

debugexe: $(DEBUG_EXE_FILE)

releaselib: $(RELEASE_LIB_FILE) 

releasedll: $(RELEASE_DLL_FILE)

releaseexe: $(RELEASE_EXE_FILE)

tolua: $(TOLUA_CPP_PKG) $(TOLUA_H_PKG)


$(DEBUG_LIB_FILE) : $(DEBUG_OBJ)
	$(CMD_LIB) $@ $(DEBUG_OBJ) $(DEBUG_LIB_FLAGS) 

$(DEBUG_DLL_FILE) : $(DEBUG_OBJ)
	$(CMD_DLL) -o $@ $(DEBUG_OBJ) $(PG) $(DEBUG_DLL_FLAGS) $(DEBUG_LIBS) 

$(DEBUG_EXE_FILE) : $(DEBUG_OBJ)
	$(CMD_EXE) -o $@ $(DEBUG_OBJ) $(PG) $(DEBUG_EXE_FLAGS) $(DEBUG_LIBS) 
	
$(RELEASE_LIB_FILE) : $(RELEASE_OBJ)
	$(CMD_LIB) $@ $(RELEASE_OBJ) $(RELEASE_LIB_FLAGS) 

$(RELEASE_DLL_FILE) : $(RELEASE_OBJ)
	$(CMD_DLL) -o $@ $(RELEASE_OBJ) $(PG) $(RELEASE_DLL_FLAGS) $(RELEASE_LIBS) 

$(RELEASE_EXE_FILE) : $(RELEASE_OBJ)
	$(CMD_EXE) -o $@ $(RELEASE_OBJ) $(PG) $(RELEASE_EXE_FLAGS) $(RELEASE_LIBS) 

################################################################
#生成 公用的 LUA H文件 CPP文件
################################################################
$(TOLUA_CPP_PKG) : %.tolua.cc: %.pkg
	$(CMD_TOLUAPP) -H $(patsubst %.cc, *.h, $@) -o $@ $^

$(TOLUA_H_PKG) : %.tolua.h: %.pkg
	$(CMD_TOLUAPP) -H $@ $^

################################################################
#生成 DEBUG 版 OBJ 文件
################################################################
$(DEBUG_OBJ_CPP) : $(DEBUG_OUT_DIR)/%.d: $(CPP_ROOT_DIR)/%.cpp
	$(call fun_create_dir, $(dir $@))
	$(CMD_CXX) -o $@ $(DEBUG_CXX_FLAGS) $(DEBUG_DEFS) $(DEBUG_INCS) $^

$(DEBUG_OBJ_TOLUA_CPP) : $(DEBUG_OUT_DIR)/%.d: $(CPP_ROOT_DIR)/%.tolua.cc $(CPP_ROOT_DIR)/%.tolua.h
	$(call fun_create_dir, $(dir $@))
	$(CMD_CXX) -o $@ $(DEBUG_CXX_FLAGS) $(DEBUG_DEFS) $(DEBUG_INCS) $<

$(DEBUG_OBJ_C) : $(DEBUG_OUT_DIR)/%.d: $(CPP_ROOT_DIR)/%.c
	$(call fun_create_dir, $(dir $@))
	$(CMD_C) -o $@ $(DEBUG_C_FLAGS) $(DEBUG_DEFS) $(DEBUG_INCS) $^
	
$(DEBUG_OBJ_S) : $(DEBUG_OUT_DIR)/%.d: $(CPP_ROOT_DIR)/%.s
	$(call fun_create_dir, $(dir $@))
	$(CMD_S) -o $@ $(DEBUG_C_FLAGS) $(DEBUG_DEFS) $(DEBUG_INCS) $^

################################################################
#生成 RELEASE 版 OBJ 文件
################################################################
$(RELEASE_OBJ_CPP) : $(RELEASE_OUT_DIR)/%.d: $(CPP_ROOT_DIR)/%.cpp
	$(call fun_create_dir, $(dir $@))
	$(CMD_CXX) -o $@ $(RELEASE_CXX_FLAGS) $(RELEASE_DEFS) $(RELEASE_INCS) $^

$(RELEASE_OBJ_TOLUA_CPP) : $(RELEASE_OUT_DIR)/%.d: $(CPP_ROOT_DIR)/%.tolua.cc $(CPP_ROOT_DIR)/%.tolua.h
	$(call fun_create_dir, $(dir $@))
	$(CMD_CXX) -o $@ $(RELEASE_CXX_FLAGS) $(RELEASE_DEFS) $(RELEASE_INCS) $<

$(RELEASE_OBJ_C) : $(RELEASE_OUT_DIR)/%.d: $(CPP_ROOT_DIR)/%.c
	$(call fun_create_dir, $(dir $@))
	$(CMD_C) -o $@ $(RELEASE_C_FLAGS) $(RELEASE_DEFS) $(RELEASE_INCS) $^
	
$(RELEASE_OBJ_S) : $(RELEASE_OUT_DIR)/%.d: $(CPP_ROOT_DIR)/%.s
	$(call fun_create_dir, $(dir $@))
	$(CMD_S) -o $@ $(RELEASE_C_FLAGS) $(RELEASE_DEFS) $(RELEASE_INCS) $^


.PHONY: touch
touch:
	touch -m *; touch -m *.*;

.PHONY: clean
clean:
ifdef CLEAN_FILE
	$(call fun_clean_file, $(CLEAN_FILE))
endif

.PHONY: cleanall
cleanall:
ifdef CLEAN_ALL_FILE
	$(call fun_clean_file, $(CLEAN_ALL_FILE))
endif