all:
	@find $(ROOT_DIR) -name "*.o" -type f -delete
	@echo -e $(CLR_YELLOW) "CLEAN " $(CLR_GRAY) "*.o" $(CLR_RESET)
	@find $(ROOT_DIR) -name "*.o.fpic" -type f -delete
	@echo -e $(CLR_YELLOW) "CLEAN " $(CLR_GRAY) "*.o.fpic" $(CLR_RESET)
	@find $(ROOT_DIR) -name "*.d" -type f -delete
	@echo -e $(CLR_YELLOW) "CLEAN " $(CLR_GRAY) "*.d" $(CLR_RESET)
	@find $(ROOT_DIR) -name "*.d.tmp" -type f -delete
	@echo -e $(CLR_YELLOW) "CLEAN " $(CLR_GRAY) "*.d.tmp" $(CLR_RESET)
	@rm -rf $(ROOT_DIR)/tmp/*
	@echo -E $(CLR_YELLOW) "CLEAN " $(CLR_GRAY) "tmp/*" $(CLR_RESET)
