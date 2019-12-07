hotelFinder: hotelFinder.o hashMap.o cityMap.o
	g++ -std=c++11 hotelFinder.o hashMap.o cityMap.o -o hotelFinder
cityMap.o: cityMap.cpp cityMap.h
	g++ -std=c++11 -c cityMap.cpp
hashMap.o: hashMap.cpp hashMap.h
	g++ -std=c++11 -c hashMap.cpp
hotelFinder.o: hotelFinder.cpp hotelFinder.h
	g++ -std=c++11 -c hotelFinder.cpp
clean:
	rm *.o hotelFinder