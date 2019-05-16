################################################################
#这个文件中的内容是相对固定不变的，如要修改，请通知AARON统一处理
################################################################
ifndef fun_loop_dir
define fun_loop_dir
	@for i in $1 ; \
	do \
		echo "make["$(MAKELEVEL)"]: "$(MAKE) $(MAKE_TARGET) $(TARGET) "-C" $$i";"; \
		$(MAKE) $(MAKE_TARGET) -C $$i || exit "$$?" ;\
	done
endef
endif

MAKE_TARGET=$@

CURRENTDIR=$(shell pwd)

target: debug release

debug:
	$(call fun_loop_dir, $(MAKESRC), $(CURRENTDIR))
	
release:
	$(call fun_loop_dir, $(MAKESRC), $(CURRENTDIR))

.DEFAULT:
	$(call fun_loop_dir, $(MAKESRC), $(CURRENTDIR))
