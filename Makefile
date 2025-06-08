LANG := c

# TOOLS
ifeq ($(LANG),c)
	CC := gcc
else
	CC := g++
endif
# OPTIONS
LEVEL ?= 3
DEBUG ?= -DNDEBUG -D_FORTIFY_SOURCE=1
OPTFL ?= -march=native -mtune=native -pipe -fno-plt
WARNINGS ?= -Wall -Werror -Wpedantic -Wunused-result
CFLAGS ?= -std=c99 $(WARNINGS) $(DEBUG) -O$(LEVEL) $(OPTFL)

# ARTIFACT
PROG := escalada
TEST := testes/run_tests.sh
# SOURCE
SRC := $(PROG).$(LANG)
TST := saidas

# GENERAL COMMANDS
.PHONY: all run clean debug test

all: $(PROG)

run: $(PROG)
	./$(PROG)

debug: LEVEL = g
debug: DEBUG = -DDEBUG -D_FORTIFY_SOURCE=2 -DCHECA_METODOS -ggdb3
debug: OPTFL = -march=native
debug: clean all

clean:
	rm -rf $(PROG) $(PROG).o $(TST)

test: $(PROG)
	$(TEST)

# BUILDING
$(PROG): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%: %.c
	$(CC) $(CFLAGS) $^ -o $@
