#include "stdint.h"

#define portMAX_DELAY 1

typedef int xQueueHandle;

void xQueueSendToBack(xQueueHandle draw_queue, uint32_t* value, int max_delay);

