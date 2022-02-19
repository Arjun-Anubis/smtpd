CC= g++
LIBS= 
HEADERDIR=header
CFLAGS=-Wall -g -I$(HEADERDIR)

SRCDIR=src
SRCS:=main.cc network.cc 
SRCS:=$(addprefix $(SRCDIR)/,$(SRCS) )

BUILDDIR=build
OBJS:=network.o 
OBJS:=$(addprefix $(BUILDDIR)/,$(OBJS) )

EXE=main

all: $(EXE)


$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(SRCDIR)/main.cc -o $(EXE) $(LIBS) $(OBJS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	$(CC) -c $(CFLAGS) $< -o $(BUILDDIR)/$(notdir $@) 

clean:
	$(RM) $(EXE) $(OBJS)
