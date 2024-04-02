FLAGS = -Werror -g

main.exe : main.o shell.o database.o database.h
	g++ -o main.exe main.o shell.o database.o

main.o : main.cpp
	g++ ${FLAGS} -c main.cpp

shell.o : shell.cpp database.h
	g++ ${FLAGS} -c shell.cpp

database.o : database.cpp database.h
	g++ ${FLAGS} -c database.cpp
