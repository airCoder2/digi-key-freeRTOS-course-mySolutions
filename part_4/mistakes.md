1. At first I had an issues with how I was sending the queue. I was putting
    the pointer to the allocated memory, when I was expected to put a pointer of
    wheatever I needed to pass. So basically, I needed to pass a pointer to the address
    of the allocated memory

2. The biggest one that took my time was however the corrupted heap error when I was
    trying to free the memory. It was because I was not allocating memory for the null
    terminator. The size I was passing to  malloc was .length() where it should have been
    .length() + 1

