#the compiler
CC=g++

#compiler flags, for extra error-and-warning intolerance
CPPFLAGS=-Wall -Wextra -pedantic -Weffc++ -Werror -g

#all these link flags is for linking Allegro 5 statically with my application
#so there is no need to send dlls along with executable and resources to distribution
A5LINKFLAGS=-lallegro-static -lallegro_font-static -lallegro_ttf-static -lallegro_image-static -lallegro_primitives-static
WIN32LINKFLAGS=-lopengl32 -lwinmm -luuid -lgdiplus -lfreetype -lole32 -lgdi32 -lkernel32 -lpsapi -lshlwapi -static-libgcc -static-libstdc++

#for my simple game lib
SGLIBFLAGS=-lsglib
INCLUDES=../sglib/src/
LIBS=../sglib/

#some shell commands
RM=rm
RMDIR=rmdir
MKDIR=mkdir
CP=cp

#directory where all source code is located
SRCDIR=src

#directory where all objects are located
#TODO: adapt this makefile to generate *.o -> build/*.o
OBJDIR=.

#directory where our final executable will be located
BINDIR=.

#directory for the distribution
DISTDIR=dist

#directory with the app's resources
RESOURCESDIR=resources

# 'make' looks for objects in the directories specified by VPATH
VPATH=$(SRCDIR);$(OBJDIR);$(BINDIR)

OBJECTS=main.o
EXECUTABLE=demo2d.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $(BINDIR)\$(EXECUTABLE) -L$(LIBS) $(OBJECTS) $(SGLIBFLAGS) $(A5LINKFLAGS) $(WIN32LINKFLAGS)

#header file dependencies
main.o: $(INCLUDES)/char_2d.hpp $(INCLUDES)/map2d.hpp

dist: $(EXECUTABLE)
	$(MKDIR) $(DISTDIR)
	$(CP) $(BINDIR)\$(EXECUTABLE) $(DISTDIR)
	$(CP) -r $(RESOURCESDIR) $(DISTDIR)
	$(CP) README $(DISTDIR)
	strip $(DISTDIR)\$(EXECUTABLE)
    
dist-zip: dist
	7z a $(DISTDIR).zip $(DISTDIR)\* -r
    
.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(BINDIR)\$(EXECUTABLE)

.PHONY: clean-dist
clean-dist:
	$(RM) -rf $(DISTDIR)
	$(RM) $(DISTDIR).zip
