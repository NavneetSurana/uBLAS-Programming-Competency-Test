output: main.cpp matrix.hpp
	g++ main.cpp matrix.hpp -o output
main.o: main.cpp
	g++ -c main.cpp
matrix.o: matrix.hpp
	g++ -c matrix.hpp
clear: 
	rm *.o *.hpp.gch output