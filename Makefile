.POSIX:

.PHONY: debug _debug
.PHONY: release _release
.PHONY: test _test
.PHONY: clean

debug:
	@$(MAKE) _debug BUILD_TYPE=DEBUG

release:
	@$(MAKE) _release BUILD_TYPE=RELEASE

TARGET_NAME = rubix-imgproc
BUILD_TYPE ?= RELEASE

# TODO: Include separate tests.mk file for all the potential test targets

CLEANUP ?= rm -f
MKDIR ?= mkdir -p
STATIC_LIB_EXT ?= lib
TARGET_EXT ?= exe
OBJ_FILE_EXT ?= o

CROSS =
GXX ?= $(CROSS)g++
OBJDUMP = $(CROSS)objdump

PATH_SRC = src/
PATH_INC = inc/
PATH_CFG = cfg/
PATH_BLD = build/
PATH_REL = $(PATH_BLD)rel/
PATH_DBG = $(PATH_BLD)dbg/
PATH_SCRIPTS = scripts/
PATH_RESULTS = $(PATH_BUILD)results/
PATH_PROFILE = $(PATH_BUILD)profile/
PATH_BENCHMARK = benchmark/

PATH_OBJ_FILES = $(PATH_DBG)
ifeq ($(BUILD_TYPE), RELEASE)
	PATH_OBJ_FILES = $(PATH_REL)
endif

TARGET = $(TARGET_NAME).$(TARGET_EXT)

BUILD_DIRS = $(PATH_BLD) $(PATH_OBJ_FILES)

# TODO: Coverage stuff
# TODO: Profiling stuff
# CppCheck Flags
#CPPCHECK_FLAGS = --check-level=exhaustive --cppcheck-build-dir=$(PATH_BUILD)
CPPCHECK_FLAGS = --cppcheck-build-dir=$(PATH_BLD)

COLORIZE_CPPCHECK_SCRIPT = $(PATH_SCRIPTS)colorize_cppcheck.py
COLORIZE_UNITY_SCRIPT = $(PATH_SCRIPTS)colorize_unity.py

SRC_FILES = $(wildcard $(PATH_SRC)*.cpp)
HDR_FILES = $(wildcard $(PATH_INC)*.hpp) $(wildcard $(PATH_INC)*.hpp)
MAIN_OBJ_FILE = $(PATH_OBJ_FILES)$(TARGET_NAME).$(OBJ_FILE_EXT)
DEP_OBJ_FILES = $(filter-out $(MAIN_OBJ_FILE), \
                   $(patsubst %.cpp, $(PATH_OBJ_FILES)%.$(OBJ_FILE_EXT), \
                     $(notdir $(SRC_FILES)) \
                   ) \
                 )
OBJ_FILES = $(DEP_OBJ_FILES) $(MAIN_OBJ_FILE)
LST_FILES = $(OBJ_FILES:.$(OBJ_FILE_EXT)=.lst)
# TODO: Test files
# TODO: External libraries

COMPILER_WARNINGS = \
      -Wall -Wextra -Wpedantic -pedantic-errors \
      -Wconversion -Wdouble-promotion -Wnull-dereference \
      -Wuseless-cast -Wcast-align=strict \
      -Wimplicit-fallthrough=3 -Wswitch-default -Wswitch-enum \
      -Wfloat-equal -Wlogical-op -Wstrict-aliasing \
      -Wuse-after-free=2 -W -Walloc-zero -Walloc-size -Wpacked \
      -Wdeprecated-declarations -Wparentheses -Wreturn-type \
      -Wuninitialized -Wmaybe-uninitialized -Wshadow \
      -Wduplicated-cond -Wduplicated-branches

SANITIZERS = -fsanitize=bool -fsanitize=undefined -fsanitize-trap
# TODO: If on Unix, also used the address sanitizer

# GCC's -fanalyzer is not suitable for C++ code, according to the GCC manual
COMPILER_STATIC_ANALYZER =

COMPILER_OPTIMIZATION_LEVEL_DEBUG = -Og -g3
COMPILER_OPTIMIZATION_LEVEL_SPEED = -O3
COMPILER_OPTIMIZATION_LEVEL_SPACE = -Os

OPTIMIZATION_LVL ?= $(COMPILER_OPTIMIZATION_LEVEL_DEBUG)
ifeq ($(BUILD_TYPE), RELEASE)
	OPTIMIZATION_LVL = $(COMPILER_OPTIMIZATION_LEVEL_SPEED)
endif

COMPILER_STANDARD = -std=c++17

INCLUDE_PATH = -I. -I$(PATH_INC) -I$(PATH_CFG)

COMMON_DEFINES ?=

DIAGNOSTIC_FLAGS ?=

CXXFLAGS ?= $(COMPILER_WARNINGS) $(SANITIZERS) $(COMPILER_STATIC_ANALYZER) \
             $(COMPILER_STANDARDS) $(OPTIMIZATION_LVL) \
             $(COMMON_DEFINES) $(INCLUDE_PATHS) -fdiagnostics-color
ifeq ($(BUILD_TYPE), RELEASE)
	CXXFLAGS += -DNDEBUG
endif

LDFLAGS += $(DIAGNOSTIC_FLAGS)

###############################################################################

_debug: $(TARGET) $(LST_FILES)
_release: $(TARGET) $(LST_FILES)

$(TARGET): $(BUILD_DIRS) $(OBJ_FILES)
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mProducing output executable:\033[0m $@..."
	@echo
	$(GXX) $(LDFLAGS) $(OBJ_FILES) -o $@

# Separate rule for the main object file because it has a different
# pre-requisite list than other object files of this project.
$(MAIN_OBJ_FILE): $(PATH_SRC)$(TARGET_NAME).cpp $(DEP_OBJ_FILES)
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mCompiling\033[0m the source file:\033[0m $<..."
	@echo
	$(GXX) $(CXXFLAGS) -c $< -o $@
	@echo "----------------------------------------"
	@echo -e "\033[36mRunning static analysis\033[0m on $<..."
	@echo
	cppcheck $(CPPCHECK_FLAGS) --template='{severity}: {file}:{line}: {message}' $< 2>&1 | tee $(PATH_BLD)cppcheck.log | python $(COLORIZE_CPPCHECK_SCRIPT)

$(PATH_OBJ_FILES)%.o: $(PATH_SRC)%.cpp $(PATH_INC)%.hpp
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mCompiling\033[0m the source file:\033[0m $<..."
	@echo
	$(GXX) $(CXXFLAGS) -c $< -o $@
	@echo "----------------------------------------"
	@echo -e "\033[36mRunning static analysis\033[0m on $<..."
	@echo
	cppcheck $(CPPCHECK_FLAGS) --template='{severity}: {file}:{line}: {message}' $< 2>&1 | tee $(PATH_BLD)cppcheck.log | python $(COLORIZE_CPPCHECK_SCRIPT)

$(PATH_OBJ_FILES)%.o: $(PATH_SRC)%.cpp $(PATH_INC)%.h
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mCompiling\033[0m the source file:\033[0m $<..."
	@echo
	$(GXX) $(CXXFLAGS) -c $< -o $@
	@echo "----------------------------------------"
	@echo -e "\033[36mRunning static analysis\033[0m on $<..."
	@echo
	cppcheck $(CPPCHECK_FLAGS) --template='{severity}: {file}:{line}: {message}' $< 2>&1 | tee $(PATH_BLD)cppcheck.log | python $(COLORIZE_CPPCHECK_SCRIPT)

$(PATH_OBJ_FILES)%.o: $(PATH_SRC)%.cpp
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mCompiling\033[0m the source file:\033[0m $<..."
	@echo
	$(GXX) $(CXXFLAGS) -c $< -o $@
	@echo "----------------------------------------"
	@echo -e "\033[36mRunning static analysis\033[0m on $<..."
	@echo
	cppcheck $(CPPCHECK_FLAGS) --template='{severity}: {file}:{line}: {message}' $< 2>&1 | tee $(PATH_BLD)cppcheck.log | python $(COLORIZE_CPPCHECK_SCRIPT)

# Produces an object dump that includes the disassembly of the executable
%.lst: %.o
	@echo
	@echo "----------------------------------------"
	@echo -e "\033[36mDisassembly\033[0m of $< into $@..."
	@echo
	$(OBJDUMP) -D $< > $@
	@echo

# Directories
$(PATH_BLD):
	$(MKDIR) $@

$(PATH_OBJ_FILES):
	$(MKDIR) $@

$(PATH_PROFILE):
	$(MKDIR) $@

clean:
	@echo
	$(CLEANUP) $(PATH_REL)*.*
	$(CLEANUP) $(PATH_DBG)*.*
	$(CLEANUP) $(PATH_BLD)*.*
	$(CLEANUP) $(TARGET)
	@echo

.PRECIOUS: %.o
