BBCLib: 
	/Volumes/arm-willtm-linux-gnueabi/bin/arm-linux-gnueabihf-gcc -Wall -fPIC -c ./src/*.c ./examples/gpio/gpio_example.c ./examples/i2c/i2c_example.c ./examples/leds/onboardleds.c ./examples/pwm/pwm_example.c ./examples/spi/spi_example.c ./examples/uart/uart_example.c ./examples/examples.c
	/Volumes/arm-willtm-linux-gnueabi/bin/arm-linux-gnueabihf-gcc -shared -Wl,-soname,libBBCLib.so -o libBBCLib.so   *.o
		
arm: 
	/Volumes/arm-willtm-linux-gnueabi/bin/arm-linux-gnueabihf-gcc -working-directory ./build -Werror -g -I./include/ -lpthread -o lasforce-c ./src/jansson/*.c ./src/lasforce/ilda/ILDA.c ./src/lasforce/serialize/lf_serialize.c ./src/lasforce/serialize/serializeCommands.c ./src/lasforce/serialize/serializeILDA.c ./src/lf.c ./src/lasforce/lf_socket.c ./src/lasforce/lf_cache.c ./src/lasforce/lf_queue.c ./src/lasforce/lf_commands.c ./src/lasforce/json/json.c

clean:
	$(RM) libBBCLib.so
	$(RM) *.o