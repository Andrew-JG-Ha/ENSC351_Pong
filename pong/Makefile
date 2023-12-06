all:
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -pthread -Werror playerClient.c hardware/gpioButton.c hardware/led.c hardware/joystick.c utility/utils.c utility/a2d_utils.c -o playerClient -lm
	cp playerClient ${HOME}/cmpt433/public/myApps/

clean:
	rm playerClient