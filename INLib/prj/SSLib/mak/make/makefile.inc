################################################################
#����ļ��е���������Թ̶�����ģ���Ҫ�޸ģ���֪ͨAARONͳһ����
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