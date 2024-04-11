LIBS_ADDITIONAL =
CMAKE = cmake
INSTALL_DIR = $(HOME)/SmartCalc_v2.0

MAX_ERRORS = 10

# Компилятор / Линковщик
CC=g++

# Пути до используемых папок
BUILD_DIR = build

all: build

build:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR); $(CMAKE) ../; $(CMAKE) --build . --target all
install: build
	cd $(BUILD_DIR); make install;
uninstall:
	@if [ -d "$(INSTALL_DIR)" ]; then \
		rm -rf $(INSTALL_DIR); \
		echo "-- Uninstalling: $(INSTALL_DIR)"; \
	else \
		echo "-- Uninstalling: Application don't install"; \
	fi

clean:
	rm -rf $(BUILD_DIR)


.PHONY: all clean rebuild
