TASKNO=5
APP=dron_dragonfly

__start__: obj dat ./${APP}
	rm -f core.* core; ./${APP}

obj:
	mkdir obj

dat:
	mkdir dat


./${APP}: obj/main.o obj/lacze_do_gnuplota.o obj/Drone.o obj/Scene.o obj/Rectangular.o obj/HexagonalPrism-6.o obj/GeometricFigure.o 
	g++ -o ./${APP} obj/main.o obj/lacze_do_gnuplota.o obj/Drone.o obj/Scene.o obj/Rectangular.o obj/HexagonalPrism-6.o obj/GeometricFigure.o 

obj/main.o: src/main.cpp inc/lacze_do_gnuplota.hh 
	g++  -Iinc -W -Wall -pedantic -c -o obj/main.o src/main.cpp

obj/Drone.o: src/Drone.cpp inc/Drone.hh inc/Rectangular.hh inc/lacze_do_gnuplota.hh inc/HexagonalPrism-6.hh inc/Vector.hh inc/GeometricFigure.hh inc/Matrix.hh  
	g++  -Iinc -W -Wall -pedantic -c -o obj/Drone.o src/Drone.cpp

obj/Scene.o: src/Scene.cpp inc/Scene.hh inc/Drone.hh inc/lacze_do_gnuplota.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/Scene.o src/Scene.cpp

obj/Rectangular.o: src/Rectangular.cpp inc/Rectangular.hh inc/Vector.hh inc/GeometricFigure.hh inc/Matrix.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/Rectangular.o src/Rectangular.cpp

obj/HexagonalPrism-6.o: src/HexagonalPrism-6.cpp inc/HexagonalPrism-6.hh inc/Vector.hh inc/GeometricFigure.hh inc/Matrix.hh  
	g++  -Iinc -W -Wall -pedantic -c -o obj/HexagonalPrism-6.o src/HexagonalPrism-6.cpp

obj/GeometricFigure.o: src/GeometricFigure.cpp inc/GeometricFigure.hh inc/Vector.hh 
	g++  -Iinc -W -Wall -pedantic -c -o obj/GeometricFigure.o src/GeometricFigure.cpp

obj/lacze_do_gnuplota.o: src/lacze_do_gnuplota.cpp inc/lacze_do_gnuplota.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/lacze_do_gnuplota.o src/lacze_do_gnuplota.cpp


clean:
	rm -f obj/* ./${APP} core.* core

help:
	@echo
	@echo " make        - tworzenie i uruchomienie aplikacji"
	@echo " make ext    - tworzenie i uruchomienie aplikacji w wersji rozszerzonej"
	@echo " make clean  - usuniecie produktow kompilacji i konsolidacji"
	@echo " make help   - wyswietla niniejsza informacje"
	@echo
