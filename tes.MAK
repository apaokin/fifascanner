
CC=cl.exe
LINK=link.exe
SOURCES=Numbers4.cpp screen.cpp main.cpp RowBeginEnd.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
.PHONY : all
all: test.exe
#.c.obj:  
#  $(CC) $(cdebug) $(cflags) $(cvars) $*.c  
all: $(SOURCES) test.exe
	
test.exe: $(OBJECTS) 
	$(LINK) /OUT:test.exe  /LIBPATH:LIB liblept168.lib libtesseract302.lib $(OBJECTS) 

.cpp.o:
	$(CC)  /Fo$@  /I include /c  $<

#test.exe: main.o
#	$(LINK) /OUT:test.exe $(OBJECTS) /LIBPATH:LIB liblept168.lib libtesseract302.lib

#$(OBJECTS): $(SOURCES) 
	
#	$(CC)  /Fo$(OBJECTS)   /I include /c  $(SRC_FILES)
#main.o: $(SRC_FILES) 
#	$(CC)  /Fomain.o   /I include /c  $(SRC_FILES)
#main.o : main.cpp Numbers4.cpp screen.cpp RowBeginEnd.cpp 
#	$(CC) /c main.cpp /c screen.cpp /c Numbers4.cpp RowBeginEnd.cpp /Fmain.o 
#screen.o: screen.cpp
#	$(CC) /c screen.cpp /Fscreen.o /I include

#number.o: Numbers.cpp
#	$(CC) /c screen.cpp /Fscreen.o /I include

#clean :
#	del main.o test.exe