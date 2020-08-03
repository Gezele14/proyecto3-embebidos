APP = server
NAME_UTIL = util
BIN = ../bin
INCLUDE = ../include
LIB = ../lib
SRC = $(APP).cpp
IFlAGS=-I$(SDKTARGETSYSROOT)/usr/include
FLAGS= -lm -lyder -lulfius

#Aplicacion con biblioteca estatica
APP: $(SRC)
		$(CXX) -O $(IFlAGS) -o $(BIN)/$(APP) $(SRC)  $(FLAGS)
