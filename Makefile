rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES=$(call rwildcard,src,*.c)
TESTS=$(call rwildcard,test,*.c)
LINKER_FLAGS=$(patsubst %.c, build/%.lf, $(TESTS))
EXAMPLES=$(call rwildcard,example,*.c)
SRC_OBJECTS=$(patsubst %.c, build/%.o, $(SOURCES))
TEST_OBJECTS=$(patsubst %.c, build/%.o, $(TESTS))
EXAMPLE_OBJECTS=$(patsubst %.c, build/%.o, $(EXAMPLES))
EXAMPLE_BINARIES=$(patsubst %.c, bin/%, $(EXAMPLES))
DEPENDENCIES=$(patsubst %.c, build/%.d, $(SOURCES)$(TESTS)$(EXAMPLES))
TEST_BIN=tests
TEST_RUN=build/$(TEST_BIN).run

MEMORY_DEBUG_FLAGS=-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract
WARN_FLAGS=-Wall -Wextra -Werror -Wno-error=cpp -Wno-unused-function -Wunused-result -Wvla -Wshadow -Wstrict-prototypes -Wno-maybe-uninitialized -Wno-logical-not-parentheses
SANTIIZER_FLAGS=-fsanitize=undefined -fsanitize-address-use-after-scope -fstack-check -fno-stack-clash-protection
DEBUG_FLAGS=$(WARN_FLAGS) $(MEMORY_DEBUG_FLAGS) $(SANITIZER_FLAGS) -Og -ggdb3 -MMD -MP
OPTIMIZE_FLAGS=-march=x86-64-v3 -O2 -pipe -D NDEBUG
LINK_FLAGS=$(BASE_CFLAGS) -s -flto=4 -fwhole-program
TEST_FLAGS=$(CFLAGS) -Isrc -lctf
BASE_CFLAGS=-std=gnu11
CFLAGS=$(BASE_CFLAGS)

all: release

install: bin/pf
	cp bin/pf /usr/bin

uninstall:
	rm /usr/bin/pf

release: CFLAGS += $(OPTIMIZE_FLAGS)
release: bin/pf

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TEST_RUN) bin/pf

check: $(TEST_RUN)

clean:
	rm -rf bin build

MAKEFLAGS += --no-builtin-rules
.SUFFIXES:
.DELETE_ON_ERROR:
.PHONY: all release debug check clean dist install uninstall
$(VERBOSE).SILENT:
$(shell mkdir -p $(dir $(DEPENDENCIES)))
-include $(DEPENDENCIES)

bin/pf: $(SRC_OBJECTS)
	mkdir -p $(@D)
	$(info LNK $@)
	$(CC) $(LINK_FLAGS) $(CFLAGS) -o $@ $^

$(TEST_RUN): bin/$(TEST_BIN)
	mkdir -p $(@D)
	$(info RUN $<)
	./$<
	touch $@

bin/$(TEST_BIN): $(TEST_OBJECTS) $(filter-out build/src/main.o, $(SRC_OBJECTS)) | build/test/$(TEST_BIN).lf
	mkdir -p $(@D)
	$(info LN  $@)
	$(CC) $(LINK_FLAGS) $(TEST_FLAGS) `cat $|` -o $@ $^

build/test/$(TEST_BIN).lf: $(TESTS)
	$(info FLG $@)
	$(SHELL) ./scripts/wraps $^ > $@

build/test/%.o: test/%.c
	mkdir -p $(@D)
	$(info CC  $@)
	$(CC) $(TEST_FLAGS) -c -o $@ $<

build/test/main.o: test/main.c
	mkdir -p $(@D)
	$(info CC  $@)
	$(CC) $(TEST_FLAGS) -c -o $@ $<

build/src/%.o: src/%.c
	mkdir -p $(@D)
	$(info CC  $@)
	$(CC) $(CFLAGS) -c -o $@ $<
