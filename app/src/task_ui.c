/*
 * Copyright (c) 2023 Sebastian Bedin <sebabedin@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 */

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "app.h"
#include "task_led.h"
/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

extern SemaphoreHandle_t hsem_button;
extern SemaphoreHandle_t hsem_led;

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

void task_ui(void *argument)
{
	while (true)
	{
		message_t message ;
		if (pdPASS == xQueueReceive(ui_event_queue, &message, pdMS_TO_TICKS(10))){



			led_color_t led;
			//if(pdTRUE == xSemaphoreTake(hsem_button, portMAX_DELAY))
			//{

			LOGGER_INFO("ui led activate");
			//xSemaphoreGive(hsem_led);
			switch (message.button) {
			case BUTTON_TYPE_NONE:
				break;
			case BUTTON_TYPE_PULSE:
				led = LED_COLOR_RED;

				xQueueSend(led_r_event_queue,&led	,	pdMS_TO_TICKS(10) );
				break;
			case BUTTON_TYPE_SHORT:
				led = LED_COLOR_GREEN;

				xQueueSend(led_g_event_queue,&led	,	pdMS_TO_TICKS(10) );
				break;
			case BUTTON_TYPE_LONG:
				led = LED_COLOR_BLUE;

				xQueueSend(led_b_event_queue,&led	,	pdMS_TO_TICKS(10) );
				break;
			default:
				LOGGER_INFO("button error");
				break;
			}



			//}
		}
	}
}

/********************** end of file ******************************************/
