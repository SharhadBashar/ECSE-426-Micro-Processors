////////////////////////////////////////////////////////////////////////////////
//	File Name				: cc2500.c
//	Description			: Functions and methods for the wireless driver
//	Course					: ECSE 426
//	Authors					: Group 09
//	Date						: December 9th, 2016
////////////////////////////////////////////////////////////////////////////////

#include "cc2500.h"
#include <stm32f4xx.h>
#include <cmsis_os.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80)
/* Multiple byte read/write command */
#define BURSTSTATUS_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

SPI_HandleTypeDef    SpiHandlerWireless;
GPIO_InitTypeDef GPIO_InitStructure;
uint32_t CC2500_TIMEOUT = 0;

//Initialize CC2500 and SPI
void cc2500_Init (){
  /* Enable CS, INT1, INT2  GPIO clock */
  __GPIOB_CLK_ENABLE();

  GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;

  GPIO_InitStructure.Pin = CC2500_SPI_SCK_PIN;
  HAL_GPIO_Init(CC2500_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.Pin =  CC2500_SPI_MOSI_PIN;
  HAL_GPIO_Init(CC2500_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.Pin = CC2500_SPI_MISO_PIN;
  HAL_GPIO_Init(CC2500_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin   = CC2500_SPI_CS_PIN;
  GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(CC2500_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  HAL_GPIO_WritePin(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN, GPIO_PIN_SET);
	
	cc2500_SPI_Init();
	cc2500_Chipset_Init();
}
//Write configurations to CC2500 Chip
void cc2500_Chipset_Init(){
	
	uint8_t buf = VAL_CC2500_IOCFG2;
	cc2500_Write(&buf, 0x00, 0x01);
	
	buf = VAL_CC2500_IOCFG0;
	cc2500_Write(&buf, 0x02, 0x01);
	
	buf = VAL_CC2500_FIFOTHR;
	cc2500_Write(&buf, 0x03, 0x01);

	buf = VAL_CC2500_PKTLEN;
	cc2500_Write(&buf, 0x06, 0x01);
	
	buf = VAL_CC2500_PKTCTRL1;
	cc2500_Write(&buf, 0x07, 0x01);
	
	buf = VAL_CC2500_PKTCTRL0;
	cc2500_Write(&buf, 0x08, 0x01);
	
	buf = VAL_CC2500_ADDR;
	cc2500_Write(&buf, 0x09, 0x01);
	
	buf = VAL_CC2500_CHANNR;
	cc2500_Write(&buf, 0x0A, 0x01);
	
	buf = VAL_CC2500_FSCTRL1;
	cc2500_Write(&buf, 0x0B, 0x01);

	buf = VAL_CC2500_FSCTRL0;
	cc2500_Write(&buf, 0x0C, 0x01);

	buf = VAL_CC2500_FREQ2;
	cc2500_Write(&buf, 0x0D, 0x01);
	
	buf = VAL_CC2500_FREQ1;
	cc2500_Write(&buf, 0x0E, 0x01);

	buf = VAL_CC2500_FREQ0;
	cc2500_Write(&buf, 0x0F, 0x01);
	
	buf = VAL_CC2500_MDMCFG4;
	cc2500_Write(&buf, 0x10, 0x01);
	
	buf = VAL_CC2500_MDMCFG3;
	cc2500_Write(&buf, 0x11, 0x01);
	
	buf = VAL_CC2500_MDMCFG2;
	cc2500_Write(&buf, 0x12, 0x01);
	
	buf = VAL_CC2500_MDMCFG1;
	cc2500_Write(&buf, 0x13, 0x01);
	
	buf = VAL_CC2500_MDMCFG0;
	cc2500_Write(&buf, 0x14, 0x01);

	buf = VAL_CC2500_DEVIATN;
	cc2500_Write(&buf, 0x15, 0x01);
	
	buf = VAL_CC2500_MCSM1;
	cc2500_Write(&buf, 0x17, 0x01);
	
	buf = VAL_CC2500_MCSM0;
	cc2500_Write(&buf, 0x18, 0x01);
	
	buf = VAL_CC2500_FOCCFG;
	cc2500_Write(&buf, 0x19, 0x01);
	
	buf = VAL_CC2500_BSCFG;
	cc2500_Write(&buf, 0x1A, 0x01);
	
	buf = VAL_CC2500_AGCTRL2;
	cc2500_Write(&buf, 0x1B, 0x01);
	
	buf = VAL_CC2500_AGCTRL1;
	cc2500_Write(&buf, 0x1C, 0x01);
	
	buf = VAL_CC2500_AGCTRL0;
	cc2500_Write(&buf, 0x1D, 0x01);
	
	buf = VAL_CC2500_FREND1;
	cc2500_Write(&buf, 0x21, 0x01);
	
	buf = VAL_CC2500_FREND0;
	cc2500_Write(&buf, 0x22, 0x01);
	
	buf = VAL_CC2500_FSCAL3;
	cc2500_Write(&buf, 0x23, 0x01);
	
	buf = VAL_CC2500_FSCAL2;
	cc2500_Write(&buf, 0x24, 0x01);

	buf = VAL_CC2500_FSCAL1;
	cc2500_Write(&buf, 0x25, 0x01);

	buf = VAL_CC2500_FSCAL0;
	cc2500_Write(&buf, 0x26, 0x01);
	
	buf = VAL_CC2500_FSTEST;
	cc2500_Write(&buf, 0x29, 0x01);
	
	buf = VAL_CC2500_TEST2;
	cc2500_Write(&buf, 0x2C, 0x01);
	
	buf = VAL_CC2500_TEST1;
	cc2500_Write(&buf, 0x2D, 0x01);
	
	buf = VAL_CC2500_TEST0;
	cc2500_Write(&buf, 0x2E, 0x01);
	
}

//Initialize SPI Settings
void cc2500_SPI_Init(){
	
	__HAL_RCC_SPI2_CLK_ENABLE();
	
  HAL_SPI_DeInit(&SpiHandlerWireless);
  SpiHandlerWireless.Instance 							  = SPI2;
  SpiHandlerWireless.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_16;
  SpiHandlerWireless.Init.Direction 					= SPI_DIRECTION_2LINES;
  SpiHandlerWireless.Init.CLKPhase 					= SPI_PHASE_1EDGE;
  SpiHandlerWireless.Init.CLKPolarity 				= SPI_POLARITY_LOW;
  SpiHandlerWireless.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLED;
  SpiHandlerWireless.Init.CRCPolynomial 			= 7;
  SpiHandlerWireless.Init.DataSize 					= SPI_DATASIZE_8BIT;
  SpiHandlerWireless.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
  SpiHandlerWireless.Init.NSS 								= SPI_NSS_SOFT;
  SpiHandlerWireless.Init.TIMode 						= SPI_TIMODE_DISABLED;
  SpiHandlerWireless.Init.Mode 							= SPI_MODE_MASTER;
	if (HAL_SPI_Init(&SpiHandlerWireless) != HAL_OK) {printf ("ERROR: Error in initialising SPI2 \n");};
  
	__HAL_SPI_ENABLE(&SpiHandlerWireless);

}

//	Initialize GPIO pins for SPI setup
void cc2500_GPIO_Init(){
	
	GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.Pin = CC2500_SPI_SCK_PIN;
  HAL_GPIO_Init(CC2500_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	
  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.Pin =  CC2500_SPI_MOSI_PIN;
  HAL_GPIO_Init(CC2500_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.Pin = CC2500_SPI_MISO_PIN;
  HAL_GPIO_Init(CC2500_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	/* SPI CS pin configuration */
  GPIO_InitStructure.Pin = CC2500_SPI_CS_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(CC2500_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

	HAL_GPIO_WritePin(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN, GPIO_PIN_SET);
}

//Places data onto the DR register of chip

void cc2500_SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data)
{ 
  /* Write in the DR register the data to be sent */
  hspi->Instance->DR = Data;
}

/**
  * @brief  Returns the most recent received data by the SPIx/I2Sx peripheral. 
  * @param  *hspi: Pointer to the SPI handle. Its member Instance can point to either SPI1, SPI2 or SPI3 
  * @retval The value of the received data.
  */
uint8_t cc2500_SPI_ReceiveData(SPI_HandleTypeDef *hspi)
{
  /* Return the data in the DR register */
  return hspi->Instance->DR;
}

/**
  * @brief  Writes one byte to the CC2500.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the CC2500.
  * @param  WriteAddr : CC2500's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void cc2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit:
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)BURSTSTATUS_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  CC2500_CS_LOW();

  /* Send the Address of the indexed register */
  cc2500_SendByte(WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    cc2500_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }

  /* Set chip select High at the end of the transmission */
  CC2500_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the CC2500.
  * @param  pBuffer : pointer to the buffer that receives the data read from the CC2500.
  * @param  ReadAddr : CC2500's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the CC2500.
  * @retval None
  */
void cc2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | BURSTSTATUS_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  CC2500_CS_LOW();

  /* Send the Address of the indexed register */
  cc2500_SendByte(ReadAddr);

  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to CC2500 (Slave device) */
    *pBuffer = cc2500_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }

  /* Set chip select High at the end of the transmission */
  CC2500_CS_HIGH();
}

//Send a byte to CC2500 Chip
uint8_t cc2500_SendByte(uint8_t byte)
{
  /* Loop while DR register in not empty */
  CC2500_TIMEOUT = CC2500_TIMEOUT_THRESH;
  while (__HAL_SPI_GET_FLAG(&SpiHandlerWireless, SPI_FLAG_TXE) == RESET)
  {
    if((CC2500_TIMEOUT--) == 0)
			break;
  }

  /* Send a Byte through the SPI peripheral */
  cc2500_SPI_SendData(&SpiHandlerWireless,  byte);

  /* Wait to receive a Byte */
  CC2500_TIMEOUT = CC2500_TIMEOUT_THRESH;
  while (__HAL_SPI_GET_FLAG(&SpiHandlerWireless, SPI_FLAG_RXNE) == RESET)
  {
    if((CC2500_TIMEOUT--) == 0) {
			break;
		}
  }

  /* Return the Byte read from the SPI bus */ 
  return cc2500_SPI_ReceiveData(&SpiHandlerWireless);
}

//Reciever a packet over wireless channel
void cc2500_ReadPacket(uint8_t* packet){
	uint8_t buf;
	//strobe
	cc2500_Read (&buf, 0x75, 0x01);
	if(buf != 0x0D){
		if (buf == 0x01)
			cc2500_Read(&buf, 0x34, 0x01);
		else if (buf == 0x11){
			cc2500_Read(&buf, 0x3A, 0x01);
			cc2500_Read(&buf, 0x34, 0x01);
		}
	}
	//Strobe
	cc2500_Read( &buf, 0x7B, 0x01);
	
	if(buf > 0){
	cc2500_Read(packet, 0x3F, VAL_CC2500_PKTLEN);
		//printf("number Recieved: %u\n", *packet);
	}
	
}

//Send a packet over wireless channel

void cc2500_SendPacket(uint8_t num) {
	uint8_t buf;
	cc2500_Read (&buf, 0x75, 0x01);
	if(buf != 0x13){
		if (buf == 0x01)
			cc2500_Read(&buf, 0x35, 0x01);
		else if (buf == 0x16){
			cc2500_Read(&buf, 0x35, 0x01);
			cc2500_Write(&num, 0x3B, 0x01);
		}
	}
	cc2500_Write(&num, 0x3B, 0x01);
	cc2500_Write(&num, 0x7A, 0x01);
	if(buf < 0x21){
		cc2500_Write(&num, 0x3F, 1);
		//printf("Number Sent: %u\n", num);
	}
	else{
		//strobe
		cc2500_Write(&num, 0x3B, 0x01);
	}
}
