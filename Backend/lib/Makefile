BIB = lcd_i2c
SRC = lcd_i2c.c gpio.c

CFLAGS = -c -Wall -O
DIRH = ../include

IFLAGS_OCV=-I$(SDKTARGETSYSROOT)/usr/include/opencv4/ -I$(DIRH)
LFLAGS_OCV=-L$(SDKTARGETSYSROOT)/usr/lib  `pkg-config --libs opencv4`


build:
		$(CC) $(CFLAGS) $(SRC) -I$(DIRH) -fPIC
		$(LD) -o lib$(BIB).so lcd_i2c.o gpio.o -shared
		$(CXX) $(CFLAGS) $(IFLAGS_OCV) recognize.cpp $(LFLAGS_OCV) -fPIC
		$(AR) rv librecognize.a recognize.o
		$(RANLIB) librecognize.a
		#$(LD) -o librecognize.so recognize.o -shared
		

clean:
		rm ./lib$(BIB).so ./lcd_i2c.o ./gpio.o

