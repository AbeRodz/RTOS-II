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
#include "active_object_led.h"

void led_task_init(LedTask_t *task, QueueHandle_t queue, void (*set_state)(led_cmd_t cmd)) 
{
    task->queue = queue;
    task->set_state = set_state;
}

void led_task_run(LedTask_t *task) 
{
    led_cmd_t cmd;
    while (true) 
    {
        if (xQueueReceive(task->queue, &cmd, portMAX_DELAY) == pdPASS) 
        {
            task->set_state(cmd);
            if (cmd == LED_CMD_ON) 
            {
                vTaskDelay(pdMS_TO_TICKS(1000));
                task->set_state(LED_CMD_OFF);
            }
        }
    }
}

/* Modify this function depending on the HW */
static void led_red_set_state(led_cmd_t cmd) 
{
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, cmd == LED_CMD_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* Modify this function depending on the HW */
static void led_green_set_state(led_cmd_t cmd) 
{
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, cmd == LED_CMD_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* Modify this function depending on the HW */
static void led_blue_set_state(led_cmd_t cmd) 
{
    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, cmd == LED_CMD_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void led_tasks_create(LedTask_t *red_task, LedTask_t *green_task, LedTask_t *blue_task) 
{
    led_r_event_queue = xQueueCreate(10, sizeof(led_cmd_t));
    led_task_init(red_task, led_r_event_queue, led_red_set_state);

    led_g_event_queue = xQueueCreate(10, sizeof(led_cmd_t));
    led_task_init(green_task, led_g_event_queue, led_green_set_state);

    led_b_event_queue = xQueueCreate(10, sizeof(led_cmd_t));
    led_task_init(blue_task, led_b_event_queue, led_blue_set_state);

    xTaskCreate((TaskFunction_t)led_task_run, "Red LED Task", configMINIMAL_STACK_SIZE, red_task, tskIDLE_PRIORITY, NULL);
    xTaskCreate((TaskFunction_t)led_task_run, "Green LED Task", configMINIMAL_STACK_SIZE, green_task, tskIDLE_PRIORITY, NULL);
    xTaskCreate((TaskFunction_t)led_task_run, "Blue LED Task", configMINIMAL_STACK_SIZE, blue_task, tskIDLE_PRIORITY, NULL);
}