/* Includes ------------------------------------------------------------------*/
#include "sd_card.h"
#include "lcd_log.h"

/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */

/* Private function prototypes -----------------------------------------------*/
//static void SystemClock_Config(void);
//static void Error_Handler(void);
//static void CPU_CACHE_Enable(void);

void sd_card()
{
	FRESULT res;                                        /* FatFs function common result code */
	uint32_t byteswritten, bytesread;                   /* File write/read counts */
	char wtext[] = "aa\n"; 								/* File write buffer */
	char btext[] = "STM333.TXT";
	char rtext[100];                                   	/* File read buffer */

	/*##-1- Link the micro SD disk I/O driver ##################################*/
	if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
	{
		/*##-2- Register the file system object to the FatFs module ##############*/
	    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
	    {
			/* FatFs Initialization Error */
	    	Error_Handler();
	    }
	    else
	    {
	        /*##-3- Create and Open a new text file object with write access #####*/
	        if(f_open(&MyFile, btext, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK)
	        {
				/* 'STM32.TXT' file Open for write Error */
	        	Error_Handler();
	        }
	        else
	        {
	        	/*##-4- Write data to the text file ################################*/
	        	sprintf((char*) wtext,"File%d.filscs�acdsv�ds", 10);
				res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
				LCD_UsrLog((char*) "Data has written to SD card2.\n");
				f_putc ('a', &MyFile);
				f_putc ('\n', &MyFile);
				f_putc ('a', &MyFile);
				f_putc ('b', &MyFile);
				f_puts(wtext, &MyFile);
				f_puts(wtext, &MyFile);

				if (res != FR_OK)
				{
					/* 'STM32.TXT' file Write or EOF Error */
					Error_Handler();
				}
				else
				{
					/*##-5- Close the open text file #################################*/
					f_close(&MyFile);
					LCD_UsrLog((char*) " Close the open text file\n");


				}
	        }
	    }
	  }
	  /*##-11- Unlink the micro SD disk I/O driver ###############################*/
	  FATFS_UnLinkDriver(SDPath);
}

static void Error_Handler(void)
{
  /* Turn LED1 on */
  BSP_LED_On(LED1);
  while(1)
  {
    BSP_LED_Toggle(LED1);
    HAL_Delay(200);
  }
}
