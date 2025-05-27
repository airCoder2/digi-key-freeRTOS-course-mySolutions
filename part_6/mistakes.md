1. So at first it was confusing. I was not seeing a clear way of doing it without increasign the priority level of the first task, which would immidiately jump to it without executing the fist task all the way.

2. Since my first solution did not include any mutexes, I was sure it was not the intended way of doing it.

3. Then after a while, Alhamdullilah I figured out that I needed to block the first function from executing all the way untill the end. What I did was basically taking the key at the beginning of the setup() function, and trying to take it again right before the end. That way, I would be able to block the execution of that function. One probelm with that solution was how now the setup() function is never removed from stack.
