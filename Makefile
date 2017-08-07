CFLAGS = -Wall -Isrc/include

SRCDIR = src
OBJDIR = obj

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

dither: $(OBJECTS)
	$(CC) -o $@ $^

analyse:
	clang $(CFLAGS) --analyze $(SOURCES)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	-rm -r $(OBJDIR) *.plist dither
