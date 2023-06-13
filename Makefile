all: configure build 

configure:
	cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release\
    -DQt5_DIR=${HOME}/Qt5.14.2/5.14.2/gcc_64/lib/cmake/Qt5\

build: configure
	cmake --build build -j8

cleanbuild:
	rm -rf build

cleanfiles:
	./clean.sh

cleanall: cleanfiles cleanbuild
clean: cleanall

.PHONY: all configure build clean cleanbuild cleanfiles clean