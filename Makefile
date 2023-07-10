BINARY = subnet
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

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(INCLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

-include $(DEPFILES)