
#include "device.h"
#include "PinNames.h"

#include "basic_types.h"
#include "diag.h" 

#include "i2c_api.h"
#include "pinmap.h"
//#include "rtl_lib.h"
#include "main.h"

/*I2C pin location:
* I2C0:
*	  - S0:  PA_1(SCL)/PA_4(SDA).
*	  - S1:  PA_22(SCL)/PA_19(SDA).
*	  - S2:  PA_29(SCL)/PA_30(SDA).
*
* I2C1:
*	  - S0:  PA_3(SCL)/PA_2(SDA). 
*	  - S1:  PA_18(SCL)/PA_23(SDA).
*	  - S2:  PA_28(SCL)/PA_27(SDA).
*/

//I2C1_SEL S0
#define MBED_I2C_MTR_SDA    PA_19
#define MBED_I2C_MTR_SCL    PA_22
#define MBED_I2C_INTB		PA_5
#define MBED_I2C_SLAVE_ADDR0    0x49
#define MBED_I2C_BUS_CLK        100000  //hz
#define I2C_DATA_MAX_LENGTH     20

uint8_t i2cdata_write[I2C_DATA_MAX_LENGTH];
uint8_t i2cdata_read[I2C_DATA_MAX_LENGTH];
uint16_t cmd;

i2c_t   i2cmaster;
//sensor command     
#define WAKE_UP 0x1102
#define CHIP_REFRESH1 0xFD8E
#define CHIP_REFRESH2 0xFE22
#define CHIP_REFRESH3 0xFE02
#define CHIP_REFRESH4 0xFD00
#define PS_MODE 0x0002
#define ALS_MODE 0x0001
#define POWER_UP 0x1102 
#define CHIP_RESET 0x1100
#define CHANGE_TIME 0x0851
#define SETTING_1 0x0F19
#define SETTING_2 0x0D10
#define INT 0x3022

char    i2cdatasrc[5] = {0x1B, 0x15, 0x16, 0x80, 0x88};


static void ePL_WriteCommand(uint16_t cmd)
{
	i2cdata_write[0] = (uint8_t)(cmd >>8);
	i2cdata_write[1] = (uint8_t)(cmd&0xFF);
  	i2c_write(&i2cmaster, MBED_I2C_SLAVE_ADDR0, &i2cdata_write[0], 2, 1);   
}  


void main(void)
{
        int result;
        int i;
        int light = 0;
        int flag = 0;
        char intertupt;

	DiagPrintf("Sensor_Init \r\n");
	i2c_init(&i2cmaster, MBED_I2C_MTR_SDA ,MBED_I2C_MTR_SCL);
	i2c_frequency(&i2cmaster,MBED_I2C_BUS_CLK);
 
        ePL_WriteCommand(WAKE_UP);
        ePL_WriteCommand(CHIP_REFRESH1);
        ePL_WriteCommand(CHIP_REFRESH2);
        ePL_WriteCommand(CHIP_REFRESH3);
        ePL_WriteCommand(CHIP_REFRESH4);
        
	ePL_WriteCommand(ALS_MODE);
        
        //ePL_WriteCommand(SETTING_1);
        //ePL_WriteCommand(SETTING_2);
        
        
	ePL_WriteCommand(CHIP_RESET);
        
        ePL_WriteCommand(POWER_UP);
        DelayMs(240);
        while(1){
            //ePL_WriteCommand(DATA_LOCK);
            i2c_write(&i2cmaster, MBED_I2C_SLAVE_ADDR0, &i2cdatasrc[0], 1, 1);  
            i2c_read(&i2cmaster, MBED_I2C_SLAVE_ADDR0, (char*)&i2cdata_read[0], 2, 1);
            i2c_write(&i2cmaster, MBED_I2C_SLAVE_ADDR0, &i2cdatasrc[1], 1, 1);  
            i2c_read(&i2cmaster, MBED_I2C_SLAVE_ADDR0, (char*)&i2cdata_read[1], 2, 1);
            i2c_write(&i2cmaster, MBED_I2C_SLAVE_ADDR0, &i2cdatasrc[2], 1, 1);  
            i2c_read(&i2cmaster, MBED_I2C_SLAVE_ADDR0, (char*)&i2cdata_read[2], 2, 1);
           // printf("ALS LOW: %d\n", i2cdata_read[1]);
            //printf("ALS HIGH: %d\n", i2cdata_read[2]);
            light = i2cdata_read[1] + i2cdata_read[2] * 256;
            printf("lightness: %d\n", light);
            //flag = (i2cdata_read[0] & 8)? 1:0;
            //int ret = (i2cdata_read[0] & 4)? 1:0;
            //printf("flag: %d\n", flag);
            //printf("ret: %d\n", ret);
            
        //ePL_WriteCommand(POWER_UP);
        DelayMs(1000);
        
        }

        
	

       
       
  
}
