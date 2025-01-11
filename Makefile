include scripts/config.mk

.PHONY: all dir clean

all: dir
	@$(MAKE) -C $(SRC_DIR) all -s

dir:
	mkdir -p $(OUTPUT)
	mkdir -p $(OUTPUT)/obj
	mkdir -p $(OUTPUT)/bin
	mkdir -p $(OUTPUT)/lib

clean:
	@rm -rf $(OUTPUT)/*
	@rmdir $(OUTPUT)