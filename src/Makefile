BUILD_DIR  ?= build
DIMENSION  ?= 3
CMAKE_OPTS ?=
JOBS       ?= $(shell nproc 2>/dev/null || echo 4)

# Sanitizer flags (use: make tsan / make asan)
TSAN_FLAGS = -DENABLE_TSAN=ON -DENABLE_ASAN=OFF
ASAN_FLAGS = -DENABLE_ASAN=ON -DENABLE_TSAN=OFF

.PHONY: all configure build run clean rebuild tsan asan

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -DDIMENSION=$(DIMENSION) $(CMAKE_OPTS)

$(BUILD_DIR)/Makefile:
	$(MAKE) configure

build: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) -j$(JOBS)

run: build
	./$(BUILD_DIR)/PhysicsSim

tsan:
	$(MAKE) configure CMAKE_OPTS="$(TSAN_FLAGS)" BUILD_DIR=build_tsan
	cmake --build build_tsan -j$(JOBS)

asan:
	$(MAKE) configure CMAKE_OPTS="$(ASAN_FLAGS)" BUILD_DIR=build_asan
	cmake --build build_asan -j$(JOBS)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all
