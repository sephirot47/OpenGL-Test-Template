all: main.exe

main.exe: main.cpp
	g++ -g main.cpp -o main.exe -lGL -lGLU -lSDL2

clean:
	rm -rf *.exe *.o
