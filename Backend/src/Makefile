APP = server
BIN = ../bin
BIB = lcd_i2c
SRC = $(APP).cpp
IFlAGS= -I../include -I$(SDKTARGETSYSROOT)/usr/include -I$(SDKTARGETSYSROOT)/usr/include/opencv4/
LDFLAGS= -L../lib -l$(BIB) -lrecognize -L$(SDKTARGETSYSROOT)/usr/lib -lm -lyder -lulfius  `pkg-config --libs opencv4`


APP: $(SRC)
		$(CXX) -O $(IFlAGS) -o $(BIN)/$(APP) $(SRC)  -Bdynamic  $(LDFLAGS) -Wall
