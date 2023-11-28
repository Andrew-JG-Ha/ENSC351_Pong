SOURCES1 = light_sampler.c utility/utils.c utility/a2d_utils.c utility/i2c_utils.c joystick.c led_matrix.c light_sensor.c analysis.c
SOURCES2 = noworky.c
TARGET1 = light_sampler
TARGET2 = noworky

CC = arm-linux-gnueabihf-gcc 
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -pthread -Werror

#compile and link the application
all: $(SOURCES1) $(SOURCES2) 
	$(CC) $(CFLAGS) $(SOURCES1) -o $(TARGET1) -lm
	$(CC) $(CFLAGS) $(SOURCES2)	-o $(TARGET2)
	cp light_sampler $(HOME)/cmpt433/public/myApps/light_sampler 
	cp noworky $(HOME)/cmpt433/public/myApps/noworky

#run the application
run: light_sampler
	./light_sampler

clean: 
	rm -rf light_sampler noworky