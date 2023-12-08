all:
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -pthread -Werror lcd.c ../utility/utils.c ../utility/a2d_utils.c -o lcd -lm
	cp lcd ${HOME}/cmpt433/public/myApps/

clean:
	rm lcd

# altered this make file to test lcd, when running got: 
# "ERROR: open() for /sys/firmware/devicetree/base/ocp/GPIO3_pinmux/pinctrl-names failed, No such file or directory"
# see if it's a beaglebone problem