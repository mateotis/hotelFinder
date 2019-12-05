hotelFinder: hotelFinder.o hashMap.o cityMap.o
	g++ hotelFinder.o hashMap.o cityMap.o -o hotelFinder
cityMap.o: cityMap.cpp cityMap.h
	g++ -c cityMap.cpp
hashMap.o: hashMap.cpp hashMap.h
	g++ -c hashMap.cpp
hotelFinder.o: hotelFinder.cpp hotelFinder.h
	g++ -c hotelFinder.cpp
clean:
	rm *.o hotelFinder