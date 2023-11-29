SOURCES := light_sampler.c utility.c sensor.c sampler.c analysis.c i2c.c ledMatrix.c joystick.c
HEADERS := utility.h sensor.h sampler.h analysis.h i2c.h ledMatrix.h joystick.h
CFLAGS := -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow
LDFLAGS := -pthread -lm
CC = arm-linux-gnueabihf-gcc

all: light_sampler noworky

light_sampler: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)
	cp light_sampler $(HOME)/cmpt433/public/myApps/light_sampler

noworky: 
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow noworky.c -o noworky
	cp noworky $(HOME)/cmpt433/public/myApps/

clean:
	rm -f light_sampler
	rm -f noworky
	rm -f *.o *~