BIN=bin
BUILD=build
TEST_DIR=bin/tests
DIRS=$(BIN) $(BUILD)
SRC=src

WARN_NO_ERROR=-Wno-error=cpp -Wno-error=suggest-attribute=const -Wno-error=suggest-attribute=pure -Wno-error=unused-variable -Wno-error=unused-function
WARN= -Wall -Wextra -Werror -Wvla -Wshadow -Wstrict-prototypes -Walloca -Wbad-function-cast -Wcast-align=strict -Wcast-qual -Wduplicated-branches -Wduplicated-cond -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-prototypes -Wmultichar -Wnested-externs -Wnull-dereference -Woverlength-strings -Wpointer-arith -Wredundant-decls -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wwrite-strings $(WARN_NO_ERROR)
NO_WARN_TESTS=-Wno-unused-parameter -Wno-incompatible-pointer-types -Wno-unused-but-set-parameter
MEMORY_DEBUG=-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract
DEBUG=$(MEMORY_DEBUG) -Og -ggdb3  -fsanitize=undefined -fsanitize-address-use-after-scope -fstack-check -fno-stack-clash-protection
RELEASE=-march=native -O2 -s -pipe -flto=4 -fwhole-program -D NDEBUG
DIST=-march=x86-64-v2 -O2 -s -pipe -flto=4 -fwhole-program -D NDEBUG
TEST_LIBS=$(shell pkg-config --cflags --libs cmocka)
CFLAGS=$(WARN) -march=native -std=gnu99
TESTS=$(wildcard $(SRC)/*.test.c $(SRC)/**/*.test.c)
BIN_TESTS=$(patsubst $(SRC)/%.test.c, $(TEST_DIR)/%.test,$(TESTS))
SOURCES=$(filter-out $(TESTS), $(wildcard $(SRC)/*.c $(SRC)/**/*.c))
OBJECTS=$(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SOURCES))
DEPENDS=$(patsubst $(SRC)/%.c,$(BUILD)/%.d,$(SOURCES))

all: release

$(shell mkdir -p $(dir $(DEPENDS)))
-include $(DEPENDS)

.PHONY: all install uninstall release debug clean check binaries tests
MAKEFLAGS := --jobs=$(shell nproc)
MAKEFLAGS += --output-sync=target
$(VERBOSE).SILENT:

install: binaries
	cp $(BIN)/pf /usr/bin

uninstall:
	rm /usr/bin/pf

release: CFLAGS += $(RELEASE)
release: binaries

debug: CFLAGS += $(DEBUG)
debug: tests binaries

dist: CFLAGS += $(DIST)
dist: binaries

tests: CFLAGS += $(TEST_LIBS)
tests: $(BIN_TESTS)

check: tests
check:
	for bin in $(BIN_TESTS); do \
		./$$bin; \
	done \

clean:
	rm -rf $(BIN) $(BUILD) $(CCACHE_DIR)

binaries: $(BIN)/pf | $(BIN)

$(BIN)/pf: $(OBJECTS) | $(BIN)
	$(CC) $(CFLAGS) -MMD -MP -o $@ $^

$(TEST_DIR)/%.test: $(SRC)/%.test.c $(SRC)/%.c | $(TEST_DIR)
	$(CC) $(CFLAGS) $(NO_WARN_TESTS) -o $@ $<
	./$@

$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

$(BIN):
	mkdir -p $(BIN)

$(BUILD):
	mkdir -p $(dir $(OBJECTS)) $(dir $(DEPENDS))

$(TEST_DIR):
	mkdir -p $(TEST_DIR)/$(subst $(SRC)/,,$(dir $(TESTS)))
