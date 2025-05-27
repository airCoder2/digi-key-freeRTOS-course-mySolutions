1. One thing that I learned is that it is not safe to pass a pointer to a varible declared inside a local variable, as when it goes out of scope it is removed from stack. so the varibale it was pointing to goes out of scope.
    A safe way of doing it would be allocating memory and then using that allocated memory
