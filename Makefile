all: build

config::
	cmake -S . -B build

build:: config
	cmake --build build

clean::
	cmake --build build --target clean

run:: run2

run1:: config
	cmake --build build --target run1

run2:: config
	cmake --build build --target run2
