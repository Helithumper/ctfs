# time

This challenge had us guessing the result of a rand() call.

When you nc the server, you recieve the following:
~~~text
Welcome to the number guessing game!
I'm thinking of a number. Can you guess it?
Guess right and you get a flag!
Enter your number: 100
Your guess was 100.
Looking for 2055569712.
Sorry. Try again, wrong guess!

~~~

Now, we were given the source code to the program:

~~~c
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

void giveFlag(void) {
    char flag[256];
    memset(flag, 0, 256);
    FILE* flag_handle = fopen("/home/h3/flag.txt", "r");
    if (flag_handle == NULL) {
        printf("Flag file not found!  Contact an H3 admin for assistance.\n");
        return;
    }
    fgets(flag, 256, flag_handle);
    fclose(flag_handle);
    printf("%s\n", flag);
}

int main(int argc, char **argv) {
    uint32_t rand_num;
    srand(time(0)); //seed with current time
    rand_num = rand();
    uint32_t ans;
    printf("Welcome to the number guessing game!\n");
    printf("I'm thinking of a number. Can you guess it?\n");
    printf("Guess right and you get a flag!\n");

    printf("Enter your number: ");
    fflush(stdout);
    scanf("%u", &ans); // get input from user
    printf("Your guess was %u.\n", ans);
    printf("Looking for %u.\n", rand_num);
    fflush(stdout);

    if (rand_num == ans) {
        printf("You won. Guess was right! Here's your flag:\n");
        giveFlag();
    } else {
        printf("Sorry. Try again, wrong guess!\n");
    }
    fflush(stdout);

    return 0;
}

~~~

As we can see here, the program is comparing our input to a rand_num generated by
~~~c
srand(time(0));
rand_num = rand();
~~~

This is notably insecure as we can use the same seed as the random number generator. In fast, this challenge is notably easier because we can comment out most of the program to make it just print out the random number:

~~~c
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

void giveFlag(void) {
    char flag[256];
    memset(flag, 0, 256);
    FILE* flag_handle = fopen("/home/h3/flag.txt", "r");
    if (flag_handle == NULL) {
        printf("Flag file not found!  Contact an H3 admin for assistance.\n");
        return;
    }
    fgets(flag, 256, flag_handle);
    fclose(flag_handle);
    printf("%s\n", flag);
}

int main(int argc, char **argv) {
    uint32_t rand_num;
    srand(time(0)); //seed with current time
    rand_num = rand();
    uint32_t ans;
    //("Welcome to the number guessing game!\n");
    //printf("I'm thinking of a number. Can you guess it?\n");
    //printf("Guess right and you get a flag!\n");

    //printf("Enter your number: ");
    fflush(stdout);
    //scanf("%u", &ans); // get input from user
    //printf("Your guess was %u.\n", ans);
    printf("%u\n", rand_num);
    fflush(stdout);

    // if (rand_num == ans) {
    //     printf("You won. Guess was right! Here's your flag:\n");
    //     giveFlag();
    // } else {
    //     printf("Sorry. Try again, wrong guess!\n");
    // }
    // fflush(stdout);

    return 0;
}
~~~

Now, if we pipe this c file into the problem, we get the following:

~~~text
Welcome to the number guessing game!
I'm thinking of a number. Can you guess it?
Guess right and you get a flag!
Enter your number: Your guess was 1525734920.
Looking for 1525734920.
You won. Guess was right! Here's your flag:
flag{g0ttem_boyz}
~~~