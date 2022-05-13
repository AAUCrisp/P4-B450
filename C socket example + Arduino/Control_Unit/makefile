Converter: converter.cpp
	g++ converter.cpp -o converter
	touch converter

ServerStart: ServerTest.c
	gcc -pthread ServerTest.c -o ServerStart
	touch ServerStart

clean:
	rm -f ServerStart
	@echo "clean completed!"
