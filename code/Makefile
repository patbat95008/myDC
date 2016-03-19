# $Id: Makefile,v 1.6 2014-02-13 18:58:45-08 - - $

MKFILE    = Makefile
DEPSFILE  = ${MKFILE}.deps
NOINCLUDE = ci clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99 -ggdb
MKDEPS    = gcc -MM
GRIND     = valgrind --leak-check=full

CSOURCE   = main.c bigint.c debug.c stack.c token.c
CHEADER   =        bigint.h debug.h stack.h token.h
OBJECTS   = ${CSOURCE:.c=.o}
EXECBIN   = mydc
SOURCES   = ${CHEADER} ${CSOURCE} ${MKFILE}
LISTSRC   = ${SOURCES} ${DEPSFILE}
LISTING   = Listing.mydc.ps
OUTPUT    = output*.lis

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

%.o : %.c
	cid + $<
	${GCC} -c $<

ci : ${SOURCES}
	cid + ${SOURCES} test*.data

lis : ${SOURCES} test
	mkpspdf ${LISTING} ${LISTSRC} ${OUTPUT}

clean :
	- rm ${OBJECTS} ${DEPSFILE} core ${OUTPUT}

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

test : ${EXECBIN}
	- ${EXECBIN} <test1.data >output1.lis 2>&1
	- ${EXECBIN} test*.data >output2.lis 2>&1
	- ${GRIND} ${EXECBIN} <test1.data >output3.lis 2>&1

deps : ${CSOURCE} ${CHEADER}
	@ echo "# ${DEPSFILE} created `date`" >${DEPSFILE}
	${MKDEPS} ${CSOURCE} | sort | uniq >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps


again :
	gmake spotless deps ci all lis

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

