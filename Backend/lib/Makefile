BIB = lcd_i2c
SRC = lcd_i2c.c gpio.c

CFLAGS = -c -Wall
DIRH = ../include

build:
		$(CC) $(CFLAGS) $(SRC) -I$(DIRH) -O -Wall -fPIC
		$(LD) -o lib$(BIB).so lcd_i2c.o gpio.o -shared
		

clean:
		rm ./lib$(BIB).so ./lcd_i2c.o ./gpio.o

