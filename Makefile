BINARY = subnet
INSTALLDIR = /usr/local/bin
CODEDIRS = src
INCDIRS = include
CC = gcc
OPT = -O0
DEPFLAGS = -MP -MD
INCLIBS = -lm
CFLAGS = -Wall -Wextra -g $(foreach D, $(INCDIRS), -I$(D)) $(OPT) $(DEPFLAGS)
CFILES = $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.c))
OBJECTS = $(patsubst %.c, %.o, $(CFILES))
DEPFILES = $(patsubst %.c, %.d, $(CFILES))

all: $(BINARY)
	@./tests_py/test.py

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(INCLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(BINARY)
	@echo "Testing invalid inputs"
	@./tests_py/test.py
	@echo "Testing for memory leaks with valgrind"
	@echo "This may take some time"
	@./tests_py/valgrind_test.py

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

install: $(BINARY)
	install -m 755 $(BINARY) $(INSTALLDIR)

uninstall:
	rm $(INSTALLDIR)/$(BINARY)

-include $(DEPFILES)