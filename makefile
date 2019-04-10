setup:
	mkdir -p build/ bin/
	make bin/CAexplorer

bin/CAexplorer: build/CAexplorer.o build/printTitle.o build/optionManager.o build/CAmanager.o build/ICA.o build/countCluster.o
	gcc build/*.o -o bin/CAexplorer

build/CAexplorer.o: src/CAexplorer.c src/printTitle.h src/optionManager.h
	gcc -c src/CAexplorer.c -o build/CAexplorer.o

build/printTitle.o: src/printTitle.c src/printTitle.h
	gcc -c src/printTitle.c -o build/printTitle.o

build/optionManager.o: src/optionManager.c src/optionManager.h src/CAmanager.h
	gcc -c src/optionManager.c -o build/optionManager.o

build/CAmanager.o: src/CAmanager.c src/CAmanager.h src/CA/*.h
	gcc -c src/CAmanager.c -o build/CAmanager.o

build/ICA.o: src/CA/ICA.c src/CA/ICA.h src/CA/tools/countCluster.h
	gcc -c src/CA/ICA.c -o build/ICA.o

build/countCluster.o: src/CA/tools/countCluster.c src/CA/tools/countCluster.h
	gcc -c src/CA/tools/countCluster.c -o build/countCluster.o
