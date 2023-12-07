SOURCES := playerClient.c utility/utils.c utility/a2d_utils.c hardware/gpioButton.c hardware/led.c hardware/joystick.c
HEADERS := utility/utils.h utility/a2d_utils.h hardware/gpioButton.h hardware/led.h hardware/joystick.h
CFLAGS := -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow
LDFLAGS := -pthread -lm
CC = arm-linux-gnueabihf-gcc

all: playerClient

playerClient: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)
	cp playerClient $(HOME)/cmpt433/public/myApps/playerClient

clean:
	rm -f playerClient
	rm -f *.o *~