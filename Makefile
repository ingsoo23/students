run: all
	student.exe

all: students.cpp dynamic_hash.o DB.o
	g++ -Wl,--stack,10000000 -Wl,--heap,10000000 -std=c++11 -o student.exe students.cpp dynamic_hash.o DB.o

dynamic_hash.o: students.h dynamic_hash.h DB.h
	g++ -c dynamic_hash.cpp

DB.o: students.h dynamic_hash.h DB.h
	g++ -c DB.cpp

clean:
	rm -rf *.o
	
