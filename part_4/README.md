> [!WARNING]
> These info might not be accurate! 


**General memory managment (without RTOS)** 

*In most systems there are 3 main memory segments (inside the RAM) during program's runtime.*

|    STACK    | --> used to store local variables. Stack Loads all the varaibles withing the current
running funciton and pops them after the function returns. The main function is usually
at the bottom of the funciton and is popped whenever it returns.

|    HEAP     | --> used as a memory pool. Could be used whenever needed.

|    STATIC   | --> used to store global variables that should persist throughout the execution.

-----------------------------------------------------

**Memory managment in the context of RTOS** 

*You can think of RTOS as a wrapper on top of what we have seen above*

|    STACK    |  --> when RTOS is in play, stack isn't used for storing local variables.According to my research,
it is only used by RTOS mostly for contet switching to save the current tasks CPU state 
and register values, so it can resume from where it left off when it is unblocked.

|    HEAP     |  --> when new tasks are created, FreeRTOS allocates memory from the heap for that task. The size
of the heap is passed as an argument by the user when creating a task. For each task, RTOS 
creates a TCB struct to hold the task info and allocates memory for the task to use as its stack 

|    STATIC   |  --> when xTaskCreateStatic() to prevent memory leaks is used, this segment is used.



