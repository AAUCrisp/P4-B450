all: Converter ram_grid_backup ServerStart

Converter: converter.cpp
	g++ converter.cpp -o converter
	touch converter

Backup: ram_grid_backup.cpp
	g++ ram_grid_backup.cpp -o ram_grid_backup
	touch ram_grid_backup

ServerStart: ServerTest.c
	gcc -pthread ServerTest.c -o ServerStart
	touch ServerStart

clean:
	rm -f ServerStart
	@echo "clean completed!"
