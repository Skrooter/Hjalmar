/*
 * This file is part of Hjalmar.
 *
 * Hjalmar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hjalmar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hjalmar.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stddef.h>

#include "work_queue.h"
#include "error_codes.h"

typedef struct {
    work_item_t work_item;
    void *data;
} queue_item_t;

static queue_item_t queue[WORK_QUEUE_SIZE] = { 0 };
static unsigned int q_items = 0;
static unsigned int q_index = 0;

int work_queue_add(work_item_t work_item, void *data)
{
    if (work_item == NULL) {
        return HJALMAR_INVALID_ARGUMENT;
    }

    if (q_items < WORK_QUEUE_SIZE) {
        int i = (q_index + q_items) % WORK_QUEUE_SIZE;
        queue[i].work_item = work_item;
        queue[i].data = data;

        q_items++;

        return HJALMAR_OK;
    }

    return HJALMAR_QUEUE_FULL;
}

void work_queue_process(void)
{
    if ((q_items != 0) && (queue[q_index].work_item != NULL)) {

        queue[q_index].work_item(queue[q_index].data);

        queue[q_index].work_item = NULL;
        queue[q_index].data = NULL;

        q_index = (q_index + 1) % WORK_QUEUE_SIZE;
        q_items--;
    }
}

unsigned int work_queue_get_items(void)
{
    return q_items;
}
