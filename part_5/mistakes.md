1. It is weird how I have never though about this before, but apparently, printf recognizes end of the string
    when it encounter a null terminator.
> [!IMPORTANT]
> Try out the following and see what happesn
>#include <stdio.h>
>#include <string.h>
>
>int main() {
>  char my_string[] = "Hello";
>  char new_string[] = "should access";
>  int length = strlen(my_string);
>
>  printf("Original string: %s\n", my_string);
>
>  my_string[length] = ' ';
>
>  printf("String after removing null terminator: %s\n", my_string);
>
>  return 0;
>}

2. For some reason, when you try to get the length using .length() in C++ of a string that has been read from the serial
    mointor, it is allways 2 more. I have not yet figured out the reason

3. I preferred not to touch C++ commands, and do everything instead in C. I also wanted to convert the entered String 
    a C style string before doing any operation on it. I initialy though char c_string[] = String.c_str(); would be enough but no!
    I had to copy it for some reason.

4. Subhanallah, the free memory allocaiton probelem aslo occured in this one. I could not remember what the issue was with the previous one off the top of my head, but as soon as I opened my 'mistakes.md' file, it clicked! That's when I realised it is very useful to keep them.

5. One more issue that I encauntered (but did not take as much time) was how I was blocking indefinitaley upon recieving the queue. I had to change max delay to 0, so the task continues its execution instead of blocking and wating for a queue to come.



