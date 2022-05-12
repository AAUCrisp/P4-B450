all: ServerStart SensorStart ActuatorStart

ServerStart: Server.c
	gcc -pthread Server.c -o ServerStart

SensorStart: Sensor.c
	gcc -pthread Sensor.c -o SensorStart

ActuatorStart: Actuator.c
	gcc -pthread Actuator.c -o ActuatorStart

clean:
rm -f ServerStart SensorStart ActuatorStart
@echo "Clean completed!"