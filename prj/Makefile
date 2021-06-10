TASKNO=5
APP=dron_dragonfly

__start__: obj dat ./${APP}
	rm -f core.* core; ./${APP}

obj:
	mkdir obj

dat:
	mkdir dat


./${APP}: obj/main.o obj/lacze_do_gnuplota.o obj/Drone.o obj/Scene.o obj/Rectangular.o obj/HexagonalPrism-6.o obj/GeometricFigure.o obj/Plateau.o obj/SharpTopHill.o obj/LongRidgeHill.o obj/UserInterface.o obj/SceneObject.o
	g++ -o ./${APP} obj/main.o obj/lacze_do_gnuplota.o obj/Drone.o obj/Scene.o obj/Rectangular.o obj/HexagonalPrism-6.o obj/GeometricFigure.o obj/Plateau.o obj/SharpTopHill.o obj/LongRidgeHill.o obj/UserInterface.o obj/SceneObject.o

obj/main.o: src/main.cpp inc/lacze_do_gnuplota.hh inc/Drone.hh inc/Scene.hh inc/UserInterface.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/main.o src/main.cpp

obj/Drone.o: src/Drone.cpp inc/Drone.hh inc/SceneObject.hh inc/Rectangular.hh inc/lacze_do_gnuplota.hh inc/HexagonalPrism-6.hh inc/Vector.hh inc/GeometricFigure.hh inc/Matrix.hh 
	g++  -Iinc -W -Wall -pedantic -c -o obj/Drone.o src/Drone.cpp

obj/Scene.o: src/Scene.cpp inc/Scene.hh inc/Drone.hh inc/lacze_do_gnuplota.hh inc/Plateau.hh inc/SharpTopHill.hh inc/LongRidgeHill.hh inc/GeometricFigure.hh inc/SceneObject.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/Scene.o src/Scene.cpp

obj/Rectangular.o: src/Rectangular.cpp inc/Rectangular.hh inc/Vector.hh inc/GeometricFigure.hh inc/Matrix.hh inc/SceneObject.hh 
	g++  -Iinc -W -Wall -pedantic -c -o obj/Rectangular.o src/Rectangular.cpp

obj/HexagonalPrism-6.o: src/HexagonalPrism-6.cpp inc/HexagonalPrism-6.hh inc/Vector.hh inc/GeometricFigure.hh inc/Matrix.hh inc/SceneObject.hh 
	g++  -Iinc -W -Wall -pedantic -c -o obj/HexagonalPrism-6.o src/HexagonalPrism-6.cpp

obj/GeometricFigure.o: src/GeometricFigure.cpp inc/GeometricFigure.hh inc/Vector.hh inc/SceneObject.hh 
	g++  -Iinc -W -Wall -pedantic -c -o obj/GeometricFigure.o src/GeometricFigure.cpp

obj/lacze_do_gnuplota.o: src/lacze_do_gnuplota.cpp inc/lacze_do_gnuplota.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/lacze_do_gnuplota.o src/lacze_do_gnuplota.cpp

obj/Plateau.o: src/Plateau.cpp inc/Plateau.hh inc/GeometricFigure.hh inc/SceneObject.hh 
	g++  -Iinc -W -Wall -pedantic -c -o obj/Plateau.o src/Plateau.cpp

obj/SharpTopHill.o: src/SharpTopHill.cpp inc/SharpTopHill.hh inc/GeometricFigure.hh inc/SceneObject.hh 
	g++  -Iinc -W -Wall -pedantic -c -o obj/SharpTopHill.o src/SharpTopHill.cpp

obj/LongRidgeHill.o: src/LongRidgeHill.cpp inc/LongRidgeHill.hh inc/GeometricFigure.hh inc/SceneObject.hh 
	g++  -Iinc -W -Wall -pedantic -c -o obj/LongRidgeHill.o src/LongRidgeHill.cpp

obj/UserInterface.o: src/UserInterface.cpp inc/UserInterface.hh inc/Scene.hh inc/Vector.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/UserInterface.o src/UserInterface.cpp

obj/SceneObject.o: src/SceneObject.cpp inc/SceneObject.hh
	g++  -Iinc -W -Wall -pedantic -c -o obj/SceneObject.o src/SceneObject.cpp

clean:
	rm -f obj/* ./${APP} core.* core

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./dron_dragonfly

help:
	@echo
	@echo " make          - tworzenie i uruchomienie aplikacji"
	@echo " make clean    - usuniecie produktow kompilacji i konsolidacji"
	@echo " make valgrind - debugowanie przy uzyciu valgrinda"
	@echo " make help     - wyswietla niniejsza informacje"
	@echo
