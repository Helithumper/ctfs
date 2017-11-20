#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    fflush(stdout);
}

int main(int argc, char **argv) {
    uintptr_t val;
    char buf[32] = "";

    printf("Welcome to the number guessing game!\n");
    printf("I'm thinking of a number. Can you guess it?\n");
    printf("Guess right and you get a flag!\n");

    printf("Enter your number: ");
    fflush(stdout);
    scanf("%31s", buf);
    val = strtol(buf, NULL, 10);

    printf("You entered %lu. Let's see if it was right...\n", val);
    fflush(stdout);

    val >>= 4;
    ((void (*)(void))val)();
}
