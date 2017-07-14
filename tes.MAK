
CC=cl.exe
LINK=link.exe
.PHONY : all
all: test.exe
test.exe : matrix.o
	$(LINK) /OUT:test.exe matrix.o /LIBPATH:LIB liblept168.lib libtesseract302.lib
matrix.o : matrix.cpp
	$(CC) /c matrix.cpp /Fomatrix.o /I include

clean :
	del main.o test.exe