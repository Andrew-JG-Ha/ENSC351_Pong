MAIN_SOURCE = gameServer.c
UTILITY_SOURCES = utility/utils.c utility/a2d_utils.c utility/i2c_utils.c
HARDWARE_SOURCES = hardware/audio.c hardware/gpioButton.c hardware/joystick.c hardware/lcd.c hardware/led.c
GAME_SOURCES = gameParser.c gameWrite.c
LOGIC_SOURCES = playerClient.c

CFLAGS := -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow
LDFLAGS := -pthread -lm
CC = arm-linux-gnueabihf-gcc

TARGET = gameServer

all: $(MAIN_SOURCE) $(UTILITY_SOURCES) $(HARDWARE_SOURCES) $(GAME_SOURCES) $(LOGIC_SOURCES)
	$(CC) $(CFLAGS) $(MAIN_SOURCE) $(UTILITY_SOURCES) $(HARDWARE_SOURCES) $(GAME_SOURCES) $(LOGIC_SOURCES) -o $(TARGET) $(LDFLAGS)
	cp gameServer $(HOME)/cmpt433/public/myApps/gameServer

run: gameServer
	./gameServer

clean:
	rm -rf gameServer 