#include <gpio.h>
//Initialize pointers: performs memory mapping, exits if mapping fails
void gpioInitPtrs()
{
    int fd = -1;
    //Loading /dev/mem into a file
    if ((fd = open("/dev/mem", O_RDWR, 0)) == -1)
        err(1, "Error opening /dev/mem");
    //Mapping GPIO base physical address
    gpfsel0 = (unsigned int *)mmap(0, getpagesize(),
                                   PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    //Check for mapping errors
    if (gpfsel0 == MAP_FAILED)
        errx(1, "Error during mapping GPIO");
    //Set regs pointers
    gpset0 = gpfsel0 + 0x7; // offset 0x1C / 4 = 0x7
    gpclr0 = gpfsel0 + 0xA; // offset 0x28 / 4 = 0xA
    gplev0 = gpfsel0 + 0xD;
}
void i2cInitPtrs()
{
    printf("init i2c pointers\n");
    int fd = -1;
    //Loading /dev/mem into a file
    if ((fd = open("/dev/mem", O_RDWR, 0)) == -1)
        err(1, "Error opening /dev/mem");
    //Mapping GPIO base physical address
    C = (unsigned int *)mmap(0, getpagesize(),
                             PROT_WRITE, MAP_SHARED, fd, I2C_BASE);
    //Check for mapping errors
    if (C == MAP_FAILED)
        errx(1, "Error during mapping i2c");
    //Set regs pointers
    S = C + 1;
    DLEN = C + 2;
    A = C + 3;
    FIFO = C + 4;
    DIV = C + 5;
    DEL = C + 6;
    CLKT = C + 7;
    printf("done i2c pointers\n");
}

void gpioSetMode(int pin, unsigned char function)
{
    *gpfsel0 &= ~((0b111 & ~function) << pin * 3);
    *gpfsel0 |= ((0b111 & function) << pin * 3);
}

void gpioWrite(int pin, unsigned char bit)
{
    if (bit)
    {
        *gpset0 = 1 << pin; //sets bit
    }
    else
    {
        *gpclr0 = 1 << pin; //clears bit
    }
}
int gpioRead(int pin)
{
    //int reg = pin / 32;
    int offset = pin % 32;
    return (*gplev0 >> offset) & 0x00000001;
}

// Function to wait for the I2C transaction to complete
void wait_i2c_done()
{
    //Wait till done, let's use a timeout just in case
    int timeout = 50;
    while ((!((*S) & BSC_S_DONE)) && --timeout)
    {
        usleep(1000);
    }
    if (timeout == 0)
        printf("wait_i2c_done() timeout. Something went wrong.\n");
    *C =  BSC_C_I2CEN|BSC_C_CLEAR;
}

void i2c_write(unsigned int data)
{
    *DLEN = 1;
    *FIFO = data;
    *S = CLEAR_STATUS;
    *C = START_WRITE;
    wait_i2c_done();
}


