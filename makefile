INCDIR= $(shell pwd)/inc
SRCDIR= $(shell pwd)/src
OBJDIR= $(shell pwd)/obj
BINDIR= $(shell pwd)/bin

VPATH = $(SRCDIR)

# FLAGS
CC := g++
CFLAGS=-c -g -Wall `root-config --cflags` -I${INCDIR}
LDFLAGS=`root-config --glibs` -lHistPainter

# File names
EXEC = $(BINDIR)/ana
FILES= $(wildcard $(SRCDIR)/*.cc)
SOURCES=$(FILES)

OBJECTS = $(FILES:$(SRCDIR)/%.cc=${OBJDIR}/%.o)
print-%  : ; @echo $* = $($*)

$(OBJDIR)/%.o: %.cc
	$(CC) $(CFLAGS) $< -o $@
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)


# To remove generated files
clean:
	rm $(BINDIR)/* $(OBJDIR)/*.o

