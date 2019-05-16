################################################################
#这个文件中的内容是相对固定不变的，如要修改，请通知AARON统一处理
################################################################
ifndef fun_loop_file
define fun_loop_file
	@for i in $1;\
	do\
		echo "make["$(MAKELEVEL)"]: "$(MAKE) $(MAKE_TARGET) "-f" $$i";"; \
		$(MAKE) $(MAKE_TARGET) -f $$i;\
	done
endef
endif

MAKE_TARGET=$@

target: debug release

debug:
	$(call fun_loop_file, $(MAKEFILE))

release:
	$(call fun_loop_file, $(MAKEFILE))

.DEFAULT:
	$(call fun_loop_file, $(MAKEFILE))
