#include <cs50.h>
#include <stdio.h>

void print_question_marks(int n);

int main(void)
{
    // Get positive integer from user
    int n;
    do
    {
        n = get_int("Width: ");
    }
    while (n < 1);
    print_question_marks(n);
}

void print_question_marks(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("?");
    }
    printf("\n");
}