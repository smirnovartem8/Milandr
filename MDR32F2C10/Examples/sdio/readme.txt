  /**
  \addtogroup sdio readme
  @{
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2020 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.1
  * @date    11/11/2020
  * @brief   Description of the sdio Example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
  @endlatexonly
@par Example Description

This example shows, how to use SDIO.
In this demo there are SDIO initialisation function, FATFS initialisation function and f_getfree function, which return size of free space of SD card. 
For this test user needs to install SD card with XS1 on demo board. User can validate the result as size in debug mode.

@par Directory "Examples/sdio" contains:

  - main.c              Main program
  - FatFs/              Generic FAT Filesystem Module library

@par Hardware and Software environment:

  - This example is intended to run on MDR1986VK01 demo board with MDR1986VK01
  microcontroller.

@par How to use.

To launch the example, you must do the following:
  - Create a project and setup all project configurations.
  - Add main.c file.
  - Add the required files from "Libraries" folder:
        MDR32F2C10_port.h
		MDR32F2C10_timer.h
		MDR32F2C10_clk.h
		MDR32F2C10_uart.h
		sdio.h
	      
  - Edit the MDR32F2C10_config.h to set appropriate run-time parameter checking level.
  - Compile and link together all .c files and load your image into the target board.
  - Run the example.

 * <h3><center>&copy; COPYRIGHT 2020 Milandr</center></h3>
 */
