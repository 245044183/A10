 /**************************************************************************//**
* @file      UiHandlerThread.c
* @brief     File that contains the task code and supporting code for the UI Thread for ESE516 Spring (Online) Edition
* @author    You! :)
* @date      2020-04-09 

******************************************************************************/


/******************************************************************************
* Includes
******************************************************************************/
#include <errno.h>
#include "asf.h"
#include "UiHandlerThread/UiHandlerThread.h"
#include "SeesawDriver/Seesaw.h"
#include "SerialConsole.h"
#include "main.h"
#include "OLED_driver/OLED_driver.h"
/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/
uiStateMachine_state uiState;
uint8_t key_act[10];
/******************************************************************************
* Forward Declarations
******************************************************************************/

/******************************************************************************
* Callback Functions
******************************************************************************/


/******************************************************************************
* Task Function
******************************************************************************/

/**************************************************************************//**
* @fn		void vUiHandlerTask( void *pvParameters )
* @brief	STUDENT TO FILL THIS
* @details 	student to fill this
                				
* @param[in]	Parameters passed when task is initialized. In this case we can ignore them!
* @return		Should not return! This is a task defining function.
* @note         
*****************************************************************************/
void vUiHandlerTask( void *pvParameters )
{
//Do initialization code here
SerialConsoleWriteString("UI Task Started!");
uiState = UI_STATE_HANDLE_BUTTONS;
TickType_t startTime =  xTaskGetTickCount(); //Get current time
uint8_t fade_intensity = 255;
//Here we start the loop for the UI State Machine
while(1)
{
	switch(uiState)
	{
		case(UI_STATE_HANDLE_BUTTONS):
		{
			int keycount = 0;
			//Do the handle buttons code here!
			keycount = SeesawGetKeypadCount();
			if (keycount>0)
				SeesawReadKeypad(key_act,keycount);
			for(int i =0;i<keycount;i++)
			{
				uint8_t currentkey = NEO_TRELLIS_SEESAW_KEY(key_act[i]>>2);
				if ((key_act[i] & 0x03) == 0x03) // button is pressed
				{
					SeesawSetLed(currentkey, 0, 0, 255);
				}
				else                              // button is released
				{
					while(fade_intensity>0)
					{
						if(xTaskGetTickCount() - startTime > 10)
						{
							startTime =  xTaskGetTickCount();
							fade_intensity = fade_intensity - 5;
							SeesawSetLed(currentkey, 0, 0, fade_intensity);
							SeesawOrderLedUpdate();							
						}
					}
					fade_intensity = 255;
				}				
				SeesawOrderLedUpdate();
			}
		break;
		}

		case(UI_STATE_IGNORE_PRESSES):
		{
		//Ignore me for now
			break;
		}

		case(UI_STATE_SHOW_MOVES):
		{
		//Ignore me as well
			break;
		}

		default: //In case of unforseen error, it is always good to sent state machine to an initial state.
			uiState = UI_STATE_HANDLE_BUTTONS;
		break;
	}

	//After execution, you can put a thread to sleep for some time.
	vTaskDelay(50);
}



}




/******************************************************************************
* Functions
******************************************************************************/