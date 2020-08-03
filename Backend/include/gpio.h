
#ifndef GPIO_HH
#define GPIO_HH
#include <sys/mman.h> //mmap
#include <err.h>      //error handling
#include <fcntl.h>    //file ops
#include <unistd.h>   //usleep
#include <stdio.h>

//ALT FUNCTIONS
#define INPUT  0
#define OUTPUT 1
#define ALT0   4
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2

#define GPIO_BASE  0x3f200000
#define I2C_BASE   0x3f804000


#define BSC_C_I2CEN     (1 << 15)
#define BSC_C_INTR      (1 << 10)
#define BSC_C_INTT      (1 << 9)
#define BSC_C_INTD      (1 << 8)
#define BSC_C_ST        (1 << 7)
#define BSC_C_CLEAR     (1 << 4)
#define BSC_C_READ      1
 
#define START_READ      BSC_C_I2CEN|BSC_C_ST|BSC_C_CLEAR|BSC_C_READ
#define START_WRITE     BSC_C_I2CEN|BSC_C_ST
 
#define BSC_S_CLKT  (1 << 9)
#define BSC_S_ERR     (1 << 8)
#define BSC_S_RXF     (1 << 7)
#define BSC_S_TXE     (1 << 6)
#define BSC_S_RXD     (1 << 5)
#define BSC_S_TXD     (1 << 4)
#define BSC_S_RXR     (1 << 3)
#define BSC_S_TXW     (1 << 2)
#define BSC_S_DONE    (1 << 1)
#define BSC_S_TA      1
 
#define CLEAR_STATUS    BSC_S_CLKT|BSC_S_ERR|BSC_S_DONE

//Regs GPIO pointers
volatile unsigned int *gpfsel0;
volatile unsigned int *gpset0;
volatile unsigned int *gpclr0;
volatile unsigned int *gplev0;

//Regs i2c pointers
volatile unsigned int *C;//control
volatile unsigned int *S;//status
volatile unsigned int *DLEN; //data length
volatile unsigned int *A; //address
volatile unsigned int *FIFO;
volatile unsigned int *DIV;
volatile unsigned int *DEL;
volatile unsigned int *CLKT;

/*Function prototypes*/

void gpioInitPtrs();
void gpioSetMode(int pin, unsigned char function);
void gpioWrite(int pin, unsigned char bit);
int gpioRead(int pin);

void i2cInitPtrs();
void wait_i2c_done();
void i2c_write(unsigned int data);

#endif

