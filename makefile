IncludeDirs = -Isrc/include
LinkDirs    = -Lsrc/lib
LinkDLLS    = -lsfml-graphics -lsfml-window -lsfml-system -mwindows

all: compile link 

compile:
	g++ $(IncludeDirs) -c main.cpp  Game.cpp
link:
	g++ main.o  game.o -o bin/main $(LinkDirs) $(LinkDLLS) 
	
clean:
	del *.o

go:
	bin/main.exe

run: compile link clean go