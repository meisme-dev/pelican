#include "task.h"

#include <arch/common/memory/vmm.h>

static size_t pid_counter = 0;

typedef struct task_queue {
  task_t *task;
  struct task_queue *next;
} task_queue;

static task_queue task_queue_head = {0};

void task_init(task_t *task) {
  if (task == NULL) {
    return;
  }

  if (task_queue_head.task == NULL) {
    task_queue_head.task = task;
  } else {
    task_queue *current_task = &task_queue_head;
    while (current_task->next != NULL) {
      current_task = current_task->next;
    }
    current_task->task = task;
  }

  task->pid = pid_counter;
  pid_counter++;

  task->root_page_table = vmm_init();
  //  task->context = ;
}
