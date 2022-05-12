all: ServerStart SensorStart ActuatorStart

ServerStart: Server.c
	gcc -pthread Server.c -o ServerStart
	touch ServerStart

SensorStart: Sensor.c
	gcc -pthread Sensor.c -o SensorStart
	touch SensorStart

ActuatorStart: Actuator.c
	gcc -pthread Actuator.c -o ActuatorStart
	touch ActuatorStart

clean:
	rm -f ServerStart SensorStart ActuatorStart
	@echo "Clean completed!"