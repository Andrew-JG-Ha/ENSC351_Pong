all:
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -pthread -Werror gpioButton.c ../utility/utils.c ../utility/a2d_utils.c -o gpioButton -lm
	cp gpioButton ${HOME}/cmpt433/public/myApps/

clean:
	rm gpioButton