# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -Wconversion -g -o3

# define any directories containing header files other than /usr/include
#
INCLUDES = #-I/home/newhall/include  -I../include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = #-L/home/newhall/lib  -L../lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = #-lmylib -lm

# define the C source files
SRCS =  ./src/main.c ./src/parser.c ./src/scanner.c ./src/types.c ./src/tables.c ./src/utils.c ./src/ast.c

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = ec #EARTECompiler

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

all: clean format $(MAIN) clean_indent
	@echo Simple compiler named ec has been compiled

$(MAIN): flex $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) -rf *.o ./src/*.o $(MAIN) ./src/parser.h ./src/parser.c ./src/scanner.c ./temp/

clean_indent:
	$(RM) -rf *~ ./src/*~

env:
	./scripts/env.sh

format:
	indent -linux ./src/*.h
	indent -linux ./src/*.c

run:
	./ec < ./tests/in/19.c

bison: src/parser.y
	bison src/parser.y

flex: bison src/scanner.l
	flex src/scanner.l

test:
	./scripts/tests.sh