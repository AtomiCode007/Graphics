# Example 13
EXE=Final_project

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
FinalProject.o: Final_project.c CSCIx229.h
fatal.o: fatal.c CSCIx229.h
loadtexbmp.o: loadtexbmp.c CSCIx229.h
print.o: print.c CSCIx229.h
project.o: project.c CSCIx229.h
errcheck.o: errcheck.c CSCIx229.h
object.o: object.c CSCIx229.h
RaceTrack.o: RaceTrack.c CSCIx229.h
FormulaOne.o: FormulaOne.c CSCIx229.h
cube.o: cube.c CSCIx229.h
tube.o: tube.c CSCIx229.h
road.o: road.c CSCIx229.h
wheel.o: wheel.c CSCIx229.h
steeringwheel.o: steeringwheel.c CSCIx229.h
wing.o: wing.c CSCIx229.h

#  Create archive
CSCIx229.a:fatal.o loadtexbmp.o print.o project.o errcheck.o object.o RaceTrack.o FormulaOne.o cube.o tube.o road.o wheel.o steeringwheel.o wing.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) $<

#  Link
Final_project:Final_project.o CSCIx229.a
	gcc -O3 -o $@ $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
