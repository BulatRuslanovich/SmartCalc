# Дополнительные библиотеки не для Mac, по умолчанию оставляем пустым, чтобы не портить сборку на Mac
LIBS_ADDITIONAL =
# Команда для открытия файла отчета о покрытии по умполчанию
CMD_OPEN = open
CMAKE = cmake
INSTALL_DIR = $(HOME)/SmartCalc_v2.0
TEXI2DVI = texi2dvi

# Максимальное количество ошибок компиляции для отображения
MAX_ERRORS = 10

# Определяем ОС
ifeq ($(OS), Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif
# Настраиваем переменные в зависимости от ОС
ifeq ($(detected_OS), Linux)
	detected_Linux := $(shell cat /etc/issue | sed -n '1p' | awk '{print $$1}')
	ifeq ($(detected_Linux), Arch)
	LIBS_ADDITIONAL =
	CMD_OPEN = xdg-open
	endif

	ifeq ($(detected_Linux), Ubuntu)
	LIBS_ADDITIONAL = -pthread
	CMD_OPEN = xdg-open
	endif

	ifeq ($(detected_Linux), Debian)
	LIBS_ADDITIONAL = -pthread
	CMD_OPEN = xdg-open
	endif
endif

# Набор цветов для красивостей
GREEN   =   \033[0;32m
RED		=   \033[0;31m
RESET	=   \033[0m

# Компилятор / Линковщик
CC=g++

# Пути до используемых папок
OBJ_DIR = obj
BUILD_DIR = build
GCOV_OBJ_DIR = obj/gcov
TESTS_OBJ_DIR = obj/check
TEST_DIR = check
MODELS_DIR = models
HELPERS_DIR = helpers
REPORT_DIR = report
DIST_DIR = smartcalc-2.0
DOC_DIR = doc
DOC_FILE = smartcalc.texi

#Цель и название исполняемого файла для тестов
TEST_EXE = tests
#Цель и название исполняемого файла для отчета о покрытии
GCOV_EXE = gcov_report

# Имя статичной библиотеки со всеми модулями проекта
MODULES_STATIC_LIBRARY = s21_calc.a
# Она же, но с флагами для отчета покрытия
MODULES_STATIC_LIBRARY_GCOV = s21_calc_gcov.a

# Дефолтное значение 1 если не задано с консоли
# Задаем так: make test MODE=4
MODE?=1
ifeq ($(MODE), 2)
	# Собираем с fsanitizer
	LFLAGS=-g -fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover \
	-fstack-protector -fanalyzer
	CFLAGS_TESTS=-Wall -Werror -Wextra -std=c++17 -c -g -fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable \
	-fno-sanitize-recover -fstack-protector -fmax-errors=$(MAX_ERRORS)
	CFLAGS += $(CFLAGS_TESTS) -fanalyzer
else
	ifeq ($(MODE), 3)
		# Собираем с отладочной информацией для valgrind
		LFLAGS=-g
		CFLAGS=-Wall -Werror -Wextra -std=c++17 -c -g -fmax-errors=$(MAX_ERRORS)
		CFLAGS_TESTS = $(CFLAGS)
	else
		# Стандартный режим
		LFLAGS=
		CFLAGS=-Wall -Werror -Wextra -std=c++17 -c -pedantic -fmax-errors=$(MAX_ERRORS)
		CFLAGS_TESTS = $(CFLAGS)
	endif
endif

# Флаги для lcov
GCOV_FLAGS =-fprofile-arcs -ftest-coverage

# Дополнительные библиотеки для линковки
LIBS = -lgtest $(LIBS_ADDITIONAL)

# Дополнительные библиотеки для линковки c отчетом о покрытии
LIBS_GCOV = $(LIBS) -lgcov

#
ALL_SUB_DIRS_WO_BUILD = $(shell find . -maxdepth 1 -type d -not -name "\." -not -name $(BUILD_DIR))
ALL_SUB_DIRS_FOR_DIST = $(shell find . -maxdepth 1 -type d -not -name "\." -not -name $(BUILD_DIR) -not -name $(DIST_DIR) -not -name $(DOC_DIR) -not -name $(TEST_DIR) -not -name $(OBJ_DIR))
# Заполняем переменные со списком директорий, исходных и исполняемых файлов
ALL_MODELS_DIRS = $(shell find $(MODELS_DIR) -type d) $(shell find $(HELPERS_DIR) -type d)
ALL_MODULES_C = $(notdir $(shell find $(ALL_MODELS_DIRS) -maxdepth 1 -name "*.cc"))
ALL_MODULES_H = $(shell find $(ALL_MODELS_DIRS) -maxdepth 1 -name "*.h")
MAIN_H = $(shell find . -maxdepth 1 -name "*.h")
ALL_H = $(ALL_MODULES_H) $(MAIN_H)
ALL_MODULES_O = $(ALL_MODULES_C:%.cc=%.o)

ALL_TESTS_DIRS = $(shell find $(TEST_DIR) -type d)
ALL_TESTS_C = $(notdir $(shell find $(ALL_TESTS_DIRS) -maxdepth 1 -name "*.cc"))
ALL_TESTS_H = $(shell find $(ALL_TESTS_DIRS) -maxdepth 1 -name "*.h")
ALL_TESTS_O = $(addprefix $(TESTS_OBJ_DIR)/, $(notdir $(ALL_TESTS_C:%.cc=%.o)))

ALL_MODULES_GCOV_O = $(addprefix $(GCOV_OBJ_DIR)/, $(notdir $(ALL_MODULES_C:.cc=.o)))

# Набор всех файлов для cpplint / cppcheck
ALL_CPP_H_FILES = $(shell find . -maxdepth 1 -type f | grep "\.(cpp|cc|h|hpp|inc)$$" -E) $(shell find $(ALL_SUB_DIRS_WO_BUILD) -type f | grep "\.(cpp|cc|h|hpp|inc)$$" -E)
ALL_CPP_H_FILES2 = $(shell find $(ALL_SUB_DIRS_WO_BUILD) -type f | grep "\.(cpp|cc|h|hpp|inc)$$" -E | grep -v qcustomplot) $(shell find . -maxdepth 1 -type f | grep "\.(cpp|cc|h|hpp|inc)$$" -E)

# Указываем список каталогов, в которых make выполняет поиск
vpath %.cc $(ALL_MODELS_DIRS) : $(ALL_TESTS_DIRS)
vpath %.o $(OBJ_DIR)

# Далее идут все цели
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
dvi:
	cd $(DOC_DIR); $(TEXI2DVI) $(DOC_FILE)
pdf:
	cd $(DOC_DIR); $(TEXI2DVI) $(DOC_FILE) --pdf
dist:
	@for dir in $(ALL_SUB_DIRS_FOR_DIST) ; do \
    mkdir -p $(DIST_DIR)/$$dir; \
	done
	@for dir in $(ALL_SUB_DIRS_FOR_DIST) ; do \
    cp -R $$dir/* $(DIST_DIR)/$$dir; \
	done
	@cp Makefile $(DIST_DIR)/Makefile
	@cp CMakeLists.txt $(DIST_DIR)/CMakeLists.txt
	@cp main.cc $(DIST_DIR)/main.cc
	@cp calc_model_facade.cc $(DIST_DIR)/calc_model_facade.cc
	@cp calc_model_facade.h $(DIST_DIR)/calc_model_facade.h
	@cp container_calc_app.cc $(DIST_DIR)/container_calc_app.cc
	@cp container_calc_app.h $(DIST_DIR)/container_calc_app.h
	@tar -czf $(DIST_DIR).tar.gz $(DIST_DIR)/
	@echo "-- Distribution: $(DIST_DIR).tar.gz"
	@rm -rf $(DIST_DIR)/

test: $(TEST_EXE)

$(TEST_EXE): $(OBJ_DIR) $(TESTS_OBJ_DIR) $(MODULES_STATIC_LIBRARY) $(ALL_TESTS_O)
	@$(CC) $(LFLAGS) $(ALL_TESTS_O) $(MODULES_STATIC_LIBRARY) $(LIBS) -o $(TEST_EXE)
	@echo "\n-------------------------------------------------------------------------------------------------\n"
	@./$(TEST_EXE)

$(GCOV_EXE): $(GCOV_OBJ_DIR) $(TESTS_OBJ_DIR) $(REPORT_DIR) $(MODULES_STATIC_LIBRARY_GCOV) $(ALL_TESTS_O) $(ALL_H) $(ALL_TESTS_H)
	$(CC) $(LFLAGS) $(ALL_TESTS_O) $(MODULES_STATIC_LIBRARY_GCOV) $(LIBS_GCOV) -o ./$(GCOV_EXE)
	@./$(GCOV_EXE)
	@gcov -o $(GCOV_OBJ_DIR) $(ALL_MODULES_C)
	@lcov -b ./ -d $(GCOV_OBJ_DIR) --gcov-tool /usr/bin/gcov -c -o output.info
	@genhtml -o $(REPORT_DIR) output.info
	@rm *.cc.gcov output.info
	$(CMD_OPEN) $(REPORT_DIR)/index.html

$(MODULES_STATIC_LIBRARY): $(OBJ_DIR) $(TESTS_OBJ_DIR) $(ALL_MODULES_O) $(ALL_H)
	@echo "$(GREEN)*$(RESET)\c"
	@ar rc $(MODULES_STATIC_LIBRARY) $(addprefix $(OBJ_DIR)/, $(ALL_MODULES_O))
	@ranlib $(MODULES_STATIC_LIBRARY)

$(MODULES_STATIC_LIBRARY_GCOV): $(GCOV_OBJ_DIR) $(TESTS_OBJ_DIR) $(ALL_MODULES_GCOV_O) $(ALL_H)
	@echo "$(GREEN)*$(RESET)\c"
	@ar rc $(MODULES_STATIC_LIBRARY_GCOV) $(ALL_MODULES_GCOV_O)
	@ranlib $(MODULES_STATIC_LIBRARY_GCOV)

%.o: %.cc $(ALL_H) $(ALL_TESTS_H)
	@echo "$(GREEN)*$(RESET)\c"
	@$(CC) $(CFLAGS) -o $(addprefix $(OBJ_DIR)/, $@) $<

$(GCOV_OBJ_DIR)/%.o : %.cc $(ALL_H) $(ALL_TESTS_H)
	@echo "$(GREEN)*$(RESET)\c"
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $@ $<

$(TESTS_OBJ_DIR)/%.o : %.cc $(ALL_H) $(ALL_TESTS_H)
	@echo "$(GREEN)*$(RESET)\c"
	@$(CC) $(CFLAGS_TESTS) -o $@ $<

$(REPORT_DIR):
	@mkdir -p $(REPORT_DIR)
	@echo "folder: $(GREEN)$(REPORT_DIR) was created$(RESET)"
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "folder: $(GREEN)$(OBJ_DIR) was created$(RESET)"
$(GCOV_OBJ_DIR):
	@mkdir -p $(GCOV_OBJ_DIR)
	@echo "folder: $(GREEN)$(GCOV_OBJ_DIR) was created$(RESET)"
$(TESTS_OBJ_DIR):
	@mkdir -p $(TESTS_OBJ_DIR)
	@echo "folder: $(GREEN)$(TESTS_OBJ_DIR) was created$(RESET)"

distclean: clean

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BUILD_DIR)
	rm -rf $(REPORT_DIR)
	rm -f *.a *.o
	rm -f *.cc.gcov *.gcno *.gcov *.gcda output.info
	rm -f ./$(GCOV_EXE) ./$(TEST_EXE)
	rm -rf $(DIST_DIR)/
	rm -f $(DIST_DIR).tar.gz

rebuild: clean all

cpplint:
	clang-format -n --verbose $(ALL_CPP_H_FILES)
#--library=qt
cppcheck:
	cppcheck --enable=all --force --suppress=missingIncludeSystem --language=c++ $(ALL_CPP_H_FILES2)
leaks:
	leaks --atExit -- ./$(TEST_EXE)
valgrind:
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./$(TEST_EXE)
#   valgrind --tool=memcheck --track-fds=yes --trace-children=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s --suppressions=../qt2.supp ./build/SmartCalcV2
.PHONY: all clean rebuild cpplint cppcheck leaks valgrind build
