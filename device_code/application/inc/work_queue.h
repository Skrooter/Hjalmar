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

#ifndef WORK_QUEUE_H_
#define WORK_QUEUE_H_

#define WORK_QUEUE_SIZE 32

typedef void (*work_item_t)(void *data);

int work_queue_add(work_item_t item, void *data);
void work_queue_process(void);
unsigned int work_queue_get_items(void);

#endif /* WORK_QUEUE_H_ */
