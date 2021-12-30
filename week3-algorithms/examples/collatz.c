#include <stdio.h>

// Prototypes
int collatz(int n);

int main(void)
{
    int steps = collatz(27);
    printf("Number of steps: %i\n", steps);

}

int collatz(int n)
{
    // base case
    if (n == 1)
        return 0;
    // even numbers
    else if ((n % 2) == 0)
        return 1 + collatz(n / 2);
    // odd numbers
    else
        return 1 + collatz(3 * n + 1);
}