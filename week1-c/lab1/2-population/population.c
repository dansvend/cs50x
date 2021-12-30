#include <cs50.h>
#include <stdio.h>

int get_start_size(void);
int get_end_size(int startSize);
int calculate_years(int startSize, int endSize);

int main(void)
{
    // Prompt for start size
    int startSize = get_start_size();

    // Prompt for end size
    int endSize = get_end_size(startSize);

    // Calculate number of years until threshold population size is reached
    int years = calculate_years(startSize, endSize);

    // Print number of years
    printf("Years: %i\n", years);
}

// Prompt for start size.
// Will continue to prompt for start size if the input is less than 9.
int get_start_size(void)
{
    int startSize;
    do
    {
        startSize = get_int("Start size: ");
    }
    while (startSize < 9);
    return startSize;
}

// Prompt for end size.
// Will continue to prompt for end size if the input is less than start size.
int get_end_size(int startSize)
{
    int endSize;
    do
    {
        endSize = get_int("End size: ");
    }
    while (endSize < startSize);
    return endSize;
}

// Calculates the number of years until threshold population size is reached
int calculate_years(int startSize, int endSize)
{
    int years = 0;
    int size = startSize;
    while (size < endSize)
    {
        size = size + (size / 3) - (size / 4);
        years++;
    }
    return years;
}