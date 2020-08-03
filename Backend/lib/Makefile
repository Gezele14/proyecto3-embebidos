#Util Library
UTIL = util
SRC_UTIL = $(UTIL).c 
OBJ_UTIL= $(UTIL).o
LIB_UTIL = lib$(UTIL).a


FLAGS= -lm

build:
	$(CC) -O -c $(SRC_UTIL) -Wall
	$(AR) rv $(LIB_UTIL) $(OBJ_UTIL)
	$(RANLIB) $(LIB_UTIL)