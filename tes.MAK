
CC=cl.exe
LINK=link.exe


.PHONY : all
all: test.exe
test.exe : main.o
	$(LINK) /OUT:test.exe main.o /LIBPATH:LIB liblept168.lib libtesseract302.lib
main.o : main.cpp Numbers4.cpp screen.cpp RowBeginEnd.cpp 
	$(CC) /c main.cpp /c screen.cpp /c Numbers4.cpp RowBeginEnd.cpp /Fmain.o /I include
screen.o: screen.cpp
	$(CC) /c screen.cpp /Fscreen.o /I include

number.o: Numbers.cpp
	$(CC) /c screen.cpp /Fscreen.o /I include

clean :
	del main.o test.exe