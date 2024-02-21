.PHONY: all clean format debug release duckdb_debug duckdb_release pull update

all: release

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJ_DIR := $(dir $(MKFILE_PATH))

ifeq ($(OS),Windows_NT)
	TEST_PATH="/test/Release/unittest.exe"
else
	TEST_PATH="/test/unittest"
endif

#### OSX config
OSX_BUILD_UNIVERSAL_FLAG=
ifneq (${OSX_BUILD_ARCH}, "")
	OSX_BUILD_UNIVERSAL_FLAG=-DOSX_BUILD_ARCH=${OSX_BUILD_ARCH}
endif
VCPKG_TOOLCHAIN_PATH?=
ifneq ("${VCPKG_TOOLCHAIN_PATH}", "")
	TOOLCHAIN_FLAGS:=${TOOLCHAIN_FLAGS} -DVCPKG_MANIFEST_DIR='${PROJ_DIR}' -DVCPKG_BUILD=1 -DCMAKE_TOOLCHAIN_FILE='${VCPKG_TOOLCHAIN_PATH}'
endif
ifneq ("${VCPKG_TARGET_TRIPLET}", "")
	TOOLCHAIN_FLAGS:=${TOOLCHAIN_FLAGS} -DVCPKG_TARGET_TRIPLET='${VCPKG_TARGET_TRIPLET}'
endif

ifeq ($(GEN),ninja)
	GENERATOR=-G "Ninja"
	FORCE_COLOR=-DFORCE_COLORED_OUTPUT=1
endif

BUILD_FLAGS=-DEXTENSION_STATIC_BUILD=1 -DBUILD_EXTENSIONS="tpch;tpcds" ${OSX_BUILD_UNIVERSAL_FLAG} ${STATIC_LIBCPP} ${TOOLCHAIN_FLAGS} -DDUCKDB_EXPLICIT_PLATFORM='${DUCKDB_PLATFORM}'

CLIENT_FLAGS :=

# These flags will make DuckDB build the extension
EXTENSION_FLAGS=\
-DDUCKDB_EXTENSION_NAMES="mysql_scanner" \
-DDUCKDB_EXTENSION_MYSQL_SCANNER_PATH="$(PROJ_DIR)" \
-DDUCKDB_EXTENSION_MYSQL_SCANNER_SHOULD_LINK=0 \
-DDUCKDB_EXTENSION_MYSQL_SCANNER_LOAD_TESTS=1 \
-DDUCKDB_EXTENSION_MYSQL_SCANNER_TEST_PATH="$(PROJ_DIR)test"

pull:
	git submodule init
	git submodule update --recursive --remote

clean:
	rm -rf build
	rm -rf testext
	cd duckdb && make clean

# Main build
debug:
	mkdir -p  build/debug && \
	cmake $(GENERATOR) $(FORCE_COLOR) $(EXTENSION_FLAGS) ${BUILD_FLAGS} ${CLIENT_FLAGS} -DCMAKE_BUILD_TYPE=Debug -S ./duckdb/ -B build/debug && \
	cmake --build build/debug --config Debug

reldebug:
	mkdir -p build/reldebug && \
	cmake $(GENERATOR) $(FORCE_COLOR) $(EXTENSION_FLAGS) ${BUILD_FLAGS} ${CLIENT_FLAGS} -DCMAKE_BUILD_TYPE=RelWithDebInfo -S ./duckdb/ -B build/reldebug && \
	cmake --build build/reldebug --config RelWithDebInfo

release:
	mkdir -p build/release && \
	cmake $(GENERATOR) $(FORCE_COLOR) $(EXTENSION_FLAGS) ${BUILD_FLAGS} ${CLIENT_FLAGS} -DCMAKE_BUILD_TYPE=Release -S ./duckdb/ -B build/release && \
	cmake --build build/release --config Release

# Main tests
test: test_release

test_release: release
	./build/release/$(TEST_PATH) "$(PROJ_DIR)test/*"

test_debug: debug
	./build/debug/$(TEST_PATH) "$(PROJ_DIR)test/*"

format:
	cp duckdb/.clang-format .
	find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format --sort-includes=0 -style=file -i
	cmake-format -i CMakeLists.txt
	rm .clang-format

update:
	git submodule update --remote --merge
