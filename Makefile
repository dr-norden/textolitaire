all: build build/textolitaire1 build/textolitaire2

build:
	cmake -S . -B build

build/textolitaire1: build
	cmake --build build --target textolitaire1

build/textolitaire2: build
	cmake --build build --target textolitaire2

clean::
	cmake --build build --target clean

run:: run1

run1:: build/textolitaire1
	./$<

run2:: build/textolitaire2
	./$<
