/**
  ******************************************************************************
  * @file    MOUSE.c
  * @author  Ashraf Suyyagh based on the MCD Application Team implementation of the MOUSE driver
	           of both the standard peripheral library drivers and the STM32F4 Cube drivers
  * @version V 1.1.0
  * @date    17-January-2016
  * @brief   This file provides a set of functions needed to manage the MOUSE
  *          MEMS accelerometer available on STM32F4-Discovery Kit.
  ******************************************************************************
	* NOTES:
	* The accelerometer packaged driver in STM32Cube drivers is highly abstracted and uses 
	* five files (accelero.h, stm32f4_discovery_accelerometer.c/.h and MOUSE/lis302dl.c/.h)
	* I have modified the drivers I written in previous years to be STM32F4Cube compatible
	* with minimum modifications to the API/Definitions
	******************************************************************************
	* BUGS/FIXES:
	* The new GPIO struct initialisation has new definitions for inputs used as external interrupt
	* sources, that is inputs are explicitly defined as input source with triggering edge. This is 
	* used to abstract the configuration of the pin as an EXTI source. The SPI initialisation for 
	* the driver sets PE0 and PE1 as INT1/INT2 for the MOUSE and configures them accordingly.
	* By default, PE0 is set as EXTI_Line0 source. It conflicts with the Push Button. A work around 
	* is either 
	  1. Modify the driver to remove the initialisation of PE0/PE1 (not recommended)
    2. Simply call HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0) AFTER initialsing the Accelerometer, this
  * resets PE0		
  */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"
	// linker cannot find these in the header
	#include "stm32f4xx_hal_spi.h"
	#include "stm32f4xx_hal.h"

/** @defgroup STM32F4_DISCOVERY_MOUSE_Private_Defines
  * @{
  */
__IO uint32_t  MOUSETimeout = MOUSE_FLAG_TIMEOUT;

 SPI_HandleTypeDef    SpiHandle;

/* Read/Write command */
#define READWRITE_CMD              ((uint8_t)0x80)
/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

#define USE_DEFAULT_TIMEOUT_CALLBACK



static uint8_t MOUSE_SendByte(uint8_t byte);
void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data);
uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi);
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_MOUSE_Private_Functions
  * @{
  */


/**
  * @brief  Set MOUSE Initialization.
  * @param  MOUSE_Config_Struct: pointer to a MOUSE_Config_TypeDef structure
  *         that contains the configuration setting for the MOUSE.
  * @retval None
  */
void MOUSE_Init(MOUSE_InitTypeDef *MOUSE_InitStruct)
{
  uint8_t ctrl = 0x00;

  /* Configure the low level interface ---------------------------------------*/
	  /* SPI configuration -------------------------------------------------------*/
	__HAL_RCC_SPI1_CLK_ENABLE();
	
  HAL_SPI_DeInit(&SpiHandle);
  SpiHandle.Instance 							  = SPI1;
  SpiHandle.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_4;
  SpiHandle.Init.Direction 					= SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase 					= SPI_PHASE_1EDGE;
  SpiHandle.Init.CLKPolarity 				= SPI_POLARITY_LOW;
  SpiHandle.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLED;
  SpiHandle.Init.CRCPolynomial 			= 7;
  SpiHandle.Init.DataSize 					= SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
  SpiHandle.Init.NSS 								= SPI_NSS_SOFT;
  SpiHandle.Init.TIMode 						= SPI_TIMODE_DISABLED;
  SpiHandle.Init.Mode 							= SPI_MODE_MASTER;
	if (HAL_SPI_Init(&SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI1 \n");};
  
	__HAL_SPI_ENABLE(&SpiHandle);
  
	/* Configure MEMS: data rate, update mode and axes */
  ctrl = (uint8_t) (MOUSE_InitStruct->Power_Mode_Output_DataRate | \
										MOUSE_InitStruct->Continous_Update           | \
										MOUSE_InitStruct->Axes_Enable);


  /* Write value to MEMS CTRL_REG4 regsister */
  MOUSE_Write(&ctrl, MOUSE_CTRL_REG4, 1);

	/* Configure MEMS: Anti-aliasing filter, full scale, self test  */
	ctrl = (uint8_t) (MOUSE_InitStruct->AA_Filter_BW | \
										MOUSE_InitStruct->Full_Scale   | \
										MOUSE_InitStruct->Self_Test);

	/* Write value to MEMS CTRL_REG5 regsister */
	MOUSE_Write(&ctrl, MOUSE_CTRL_REG5, 1);
}



/**
  * @brief  Reads a block of data from the MOUSE.
  * @param  pBuffer : pointer to the buffer that receives the data read from the MOUSE.
  * @param  ReadAddr : MOUSE's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the MOUSE.
  * @retval None
  */
void MOUSE_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  MOUSE_CS_LOW();

  /* Send the Address of the indexed register */
  MOUSE_SendByte(ReadAddr);

  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to MOUSE (Slave device) */
    *pBuffer = MOUSE_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }

  /* Set chip select High at the end of the transmission */
  MOUSE_CS_HIGH();
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
static uint8_t MOUSE_SendByte(uint8_t byte)
{
  /* Loop while DR register in not empty */
  MOUSETimeout = MOUSE_FLAG_TIMEOUT;
  while (__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_TXE) == RESET)
  {
    if((MOUSETimeout--) == 0) return MOUSE_TIMEOUT_UserCallback();
  }

  /* Send a Byte through the SPI peripheral */
  SPI_SendData(&SpiHandle,  byte);

  /* Wait to receive a Byte */
  MOUSETimeout = MOUSE_FLAG_TIMEOUT;
  while (__HAL_SPI_GET_FLAG(&SpiHandle, SPI_FLAG_RXNE) == RESET)
  {
    if((MOUSETimeout--) == 0) {
			return MOUSE_TIMEOUT_UserCallback();
		}
  }

  /* Return the Byte read from the SPI bus */ 
  return SPI_ReceiveData(&SpiHandle);
}


#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t MOUSE_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
//  while (1)
 // {
  //}
	return 0;
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi){
	GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the SPI periph */
  __SPI1_CLK_ENABLE();

  /* Enable SCK, MOSI and MISO GPIO clocks */
  __GPIOA_CLK_ENABLE();

  /* Enable CS, INT1, INT2  GPIO clock */
  __GPIOE_CLK_ENABLE();

  //GPIO_PinAFConfig(MOUSE_SPI_SCK_GPIO_PORT, MOUSE_SPI_SCK_SOURCE, MOUSE_SPI_SCK_AF);
  //GPIO_PinAFConfig(MOUSE_SPI_MISO_GPIO_PORT, MOUSE_SPI_MISO_SOURCE, MOUSE_SPI_MISO_AF);
  //GPIO_PinAFConfig(MOUSE_SPI_MOSI_GPIO_PORT, MOUSE_SPI_MOSI_SOURCE, MOUSE_SPI_MOSI_AF);

  GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.Pin = MOUSE_SPI_SCK_PIN;
  HAL_GPIO_Init(MOUSE_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.Pin =  MOUSE_SPI_MOSI_PIN;
  HAL_GPIO_Init(MOUSE_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.Pin = MOUSE_SPI_MISO_PIN;
  HAL_GPIO_Init(MOUSE_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin   = MOUSE_SPI_CS_PIN;
  GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(MOUSE_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  HAL_GPIO_WritePin(MOUSE_SPI_CS_GPIO_PORT, MOUSE_SPI_CS_PIN, GPIO_PIN_SET);

  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.Pin   = MOUSE_SPI_INT1_PIN;
  GPIO_InitStructure.Mode  = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(MOUSE_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = MOUSE_SPI_INT2_PIN;
  HAL_GPIO_Init(MOUSE_SPI_INT2_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Transmits a Data through the SPIx/I2Sx peripheral.
  * @param  *hspi: Pointer to the SPI handle. Its member Instance can point to either SPI1, SPI2 or SPI3 
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data)
{ 
  /* Write in the DR register the data to be sent */
  hspi->Instance->DR = Data;
}

/**
  * @brief  Returns the most recent received data by the SPIx/I2Sx peripheral. 
  * @param  *hspi: Pointer to the SPI handle. Its member Instance can point to either SPI1, SPI2 or SPI3 
  * @retval The value of the received data.
  */
uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi)
{
  /* Return the data in the DR register */
  return hspi->Instance->DR;
}



