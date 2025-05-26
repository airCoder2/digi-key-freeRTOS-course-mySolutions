@> [!IMPORTANT]
> Bismillah

1. Knowing that the Serial monitor does not flush your code immidiately if you do not take it was helpful. What that meant is, I could wait until the next vTaskDelay to occur on the blink function and then change the interval. 
2. I had an issue with the stack size that I was allocated in the beginning. A quick google search revelaed that 1024 bytes might not be enough. Then I allocated 4096 for both tasks.
