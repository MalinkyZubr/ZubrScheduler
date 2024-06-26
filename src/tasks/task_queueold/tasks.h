#ifndef TASKS_H
#define TASKS_H

#include <cstdint>

#include "utils.h"

#define TMC_RESORT_THRESHOLD 4

enum ReturnFlag {
    TASK_AWAITING_SHARED_RESOURCE,
    TASK_NOT_COMPLETE,
    TASK_ERROR,
    TASK_COMPLETE,
    TASK_CREATED,
    TASK_AWAITING_EXECUTION,
    TASK_INTERRUPTED
};

typedef ReturnFlag (*Operation)(TaskManager *task_manager, Task *task);

typedef struct TaskRuntimeData {
    int execution_count;
    int current_execution_ticks;
    int ticks_from_last_execution; // 
    int ticks_from_last_action; // ticks since the task last occupied a single tick
    int adjusted_priority;

    ReturnFlag state;
} Task;

typedef struct Task {
    const uint8_t priority; // priority value from 0 to 255
    const int tick_interval; // how many ticks apart should each execution be, ideally
    const Operation task_opt; // what function should be run by task
    const int max_executions; // how many times should the task execute before it fails? 
    const int ticks_per_execution; // how many ticks is the task allowed to use in a single execution?
    const uint8_t repeat; 

    AssociatedNodeWrapper *associated_node = nullptr;

    TaskRuntimeData runtime_data;
} Task;


typedef struct AssociatedNodeWrapper {
    TaskNode* associated_node = nullptr;
    TaskQueue *task_queue = nullptr;
    TaskNode* next = nullptr;
} AssociatedNodeWrapper;


typedef struct TaskNode {
    Task *lower_priority = nullptr;
    Task *higher_priority = nullptr;
    Task *owned_task = nullptr;
} TaskNode;


typedef struct TaskQueue {
    TaskNode *lowest_priority = nullptr;
    TaskNode *highest_priority = nullptr;
} TaskQueue;

typedef struct TaskManager {
    int ticks_from_resort = 0; // ticks since adjusted priorities were calculated and resorted
    int resort_threshold = TMC_RESORT_THRESHOLD; // the number of ticks that should elapse before recalculation of priorities and resorting.
    TaskNode *currently_occupied = nullptr;
    TaskQueue task_queue;
} TaskManager;

TaskRuntimeData generate_task_runtime_data(int execution_count, int current_execution_ticks, int ticks_from_last_execution, ReturnFlag return_flag);

Task *generate_task(const uint8_t priority, const int tick_interval, const Operation task_opt, const int max_executions, const uint8_t repeat);

uint8_t calculate_task_ticks_execution(uint8_t priority);

int check_task_within_tick_limit(const Task *task);

int increment_task_execution_count(Task *task);

int increment_task_current_execution_ticks(Task *task);

int increment_task_ticks_from_last_execution(Task *task);

int check_task_previous_state(Task *task, TaskManager *task_manager);

int dynamic_task_priority_calculator(Task *task);

void pi_queue_sort(TaskQueue *task_queue);

void pi_queue_add_task(TaskNode *task_node, TaskQueue *task_queue);

void pi_queue_delete_task(TaskNode *task_node, TaskQueue *task_queue);

TaskNode *pi_queue_find_node(Task *task, TaskQueue *task_queue);

void pi_queue_destructor(TaskQueue *task_queue);

TaskNode *t_node_create(Task *task, TaskQueue *task_queue);

void a_nodes_append(Task *task, TaskNode *task_node, TaskQueue *task_queue);

void a_nodes_delete(Task *task, TaskNode *task_node);

void a_nodes_delete_all(Task *task);

void task_manager_main_isr(TaskManager *task_manager);

#endif