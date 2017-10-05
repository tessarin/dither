CFLAGS = -Wall -Isrc/include
LDLIBS = -lpng

SRCDIR = src
OBJDIR = obj

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: dither doc

dither: $(OBJECTS)
	$(CC) $^ $(LDLIBS) -o $@

analyse:
	clang $(CFLAGS) --analyze $(SOURCES)

debug: CFLAGS += -g
debug: dither

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

doc: dither.1

dither.1: dither.pod
	@echo -- Generating manpage from POD:
	pod2man --section=1 --center="dither" $? $@

.PHONY: clean

clean:
	-rm -rf $(OBJDIR) *.plist dither
	-rm -rf *.1
