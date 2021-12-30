#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Promts user for their name
    string name = get_string("What is your name?\n");

    // Greets the user
    printf("hello, %s\n", name);
}