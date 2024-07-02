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

#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "app.h"
#include "task_button.h"
#include "task_led.h"
#include "task_ui.h"

/********************** macros and definitions *******************************/


/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration *****************************/

SemaphoreHandle_t hsem_button;
SemaphoreHandle_t hsem_led;

QueueHandle_t ui_event_queue;

QueueHandle_t led_r_event_queue;
QueueHandle_t led_g_event_queue;
QueueHandle_t led_b_event_queue;

/********************** external functions definition ************************/
void app_init(void)
{

	ui_event_queue = xQueueCreate(1, sizeof(message_t));
	configASSERT(NULL != ui_event_queue);

	led_r_event_queue = xQueueCreate(1, sizeof(message_t));
	configASSERT(NULL != led_r_event_queue);

	led_g_event_queue = xQueueCreate(1, sizeof(message_t));
	configASSERT(NULL != led_g_event_queue);

	led_b_event_queue = xQueueCreate(1, sizeof(message_t));
	configASSERT(NULL != led_b_event_queue);

	hsem_button = xSemaphoreCreateBinary();
	while(NULL == hsem_button)
	{

	}

	hsem_led = xSemaphoreCreateBinary();
	while(NULL == hsem_led)
	{

	}

	BaseType_t status;

	status = xTaskCreate(task_button, "task_button", 128, NULL, tskIDLE_PRIORITY, NULL);
	while (pdPASS != status)
	{
		// error
	}

	status = xTaskCreate(task_ui, "task_ui", 128, NULL, tskIDLE_PRIORITY, NULL);
	while (pdPASS != status)
	{
		// error
	}

	status = xTaskCreate(task_led, "task_led", 128, NULL, tskIDLE_PRIORITY, NULL);
	while (pdPASS != status)
	{
		// error
	}

	LOGGER_INFO("app init");

	cycle_counter_init();
}

/********************** end of file ******************************************/
