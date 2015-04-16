all:
	g++ -c -o util.o util.cpp
	g++ -c -o classes.o classes.cpp
	g++ -c -o gl.o gl.cpp
	g++ -o gl util.o classes.o gl.o -lGLU -lGL -lglut

clean:
	rm *.o
