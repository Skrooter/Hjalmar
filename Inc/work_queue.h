
#ifndef WORK_QUEUE_H_
#define WORK_QUEUE_H_

#define WORK_QUEUE_SIZE 5

typedef void (*work_item_t)(void *data);

int work_queue_add(work_item_t item, void *data);
void work_queue_process(void);
unsigned int work_queue_get_items(void);

#endif /* WORK_QUEUE_H_ */