# project name (generate executable with this name)
TARGET   = raytracing

CC       = gcc
# compiling flags here
CFLAGS   = -Wall -Wextra -O3 -Ithird_party/libattopng

LINKER   = gcc
# linking flags here
LFLAGS   = -Wall -Wextra -lraylib -lm

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = .obj
BINDIR   = bin


SOURCES  := $(wildcard $(SRCDIR)/*.c) 
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o) 


$(BINDIR)/$(TARGET): $(OBJECTS) $(OBJDIR)/libattopng.o
	@$(LINKER) $(OBJECTS) $(OBJDIR)/libattopng.o $(LFLAGS) -o $@
	@echo "Linking complete!"
	

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "comiling $< complete"

$(OBJDIR)/libattopng.o : third_party/libattopng/libattopng.c
	@$(CC) $(CFLAGS) -c third_party/libattopng/libattopng.c -o $(OBJDIR)/libattopng.o
	@echo "comiling $libattopng.o complete"

.PHONY : clean 
clean : 
	@rm -rf $(OBJDIR)/*
	@echo cleaning complete

.PHONY : delete
delete : 
	@rm -rf $(BINDIR)/*
	@echo remved binaries

