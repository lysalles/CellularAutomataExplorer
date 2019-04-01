setup:
	mkdir -p build/ bin/
	make bin/CAexplorer

bin/CAexplorer: build/CAexplorer.o build/printTitle.o build/optionManager.o
	gcc build/CAexplorer.o build/printTitle.o build/optionManager.o -o bin/CAexplorer

build/CAexplorer.o: src/CAexplorer.c src/printTitle.h src/optionManager.h
	gcc -c src/CAexplorer.c -o build/CAexplorer.o

build/printTitle.o: src/printTitle.c src/printTitle.h
	gcc -c src/printTitle.c -o build/printTitle.o

build/optionManager.o: src/optionManager.c src/optionManager.h
	gcc -c src/optionManager.c -o build/optionManager.o

build/ICA.o: src/CA/ICA.c src/CA/ICA.h
	gcc -c src/CA/ICA.c -o build/ICA.o
