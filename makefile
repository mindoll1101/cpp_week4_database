FLAGS = -Werror -std=c++11

main.exe: main.o database.o util.o database.h
	g++ -o main.exe main.o database.o util.o

main.o: main.cpp
	g++ ${FLAGS} -c main.cpp

database.o: database.cpp database.h
	g++ ${FLAGS} -c database.cpp

util.o: util.cpp database.h
	g++ ${FLAGS} -c util.cpp

clean:
	rm *.o
	rm main
