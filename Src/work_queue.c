#include <stdint.h>
#include <stddef.h>
#include "work_queue.h"

typedef struct {
    work_item_t work_item;
    void *data;
} queue_item_t;

static queue_item_t queue[WORK_QUEUE_SIZE] = { 0 };
static unsigned int q_items = 0;
static unsigned int q_index = 0;

int work_queue_add(work_item_t work_item, void *data)
{
    if ((q_items < WORK_QUEUE_SIZE) &&
        (work_item != NULL))
    {
        queue[q_index].work_item = work_item;
        queue[q_index].data = data;

        q_items++;

        return 0;
    }

    return -1;
}

void work_queue_process(void)
{
    if ((q_items != 0) &&
        (queue[q_index].work_item != NULL))
    {
        queue[q_index].work_item(queue[q_index].data);

        queue[q_index].work_item = NULL;
        queue[q_index].data = NULL;

        q_index++;
        q_items--;

        if (q_index >= WORK_QUEUE_SIZE)
        {
            q_index = 0;
        }
    }
}

unsigned int work_queue_get_items(void)
{
    return q_items;
}