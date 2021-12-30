#include <cs50.h>
#include <stdio.h>

// Prototypes
int get_pyramid_heigh(void);
void print_pyramids(int height);
void print_spaces(int n);
void print_hashes(int n);

int main(void)
{
    // Prompt user for pyramid height between 1 and 8, inclusive
    int height = get_pyramid_heigh();

    // Prints out a left aligned pyramid, a gap and a right aligned pyramid of hashes with the height from the argument to the terminal
    print_pyramids(height);
}

// Will prompt the user for a positive integer between 1 and 8, inclusive.
int get_pyramid_heigh(void)
{
    int height;
    do
    {
        height = get_int("Heigh: ");
    }
    while (height < 1 || height > 8);
    return height;
}

// Prints out a left aligned pyramid, a gap and a right aligned pyramid of hashes with the height from the argument to the terminal
void print_pyramids(int height)
{
    for (int row = 1; row <= height; row++)
    {
        int spaces = height - row;
        // Left pyramid
        print_spaces(spaces);
        print_hashes(row);

        // Gap
        print_spaces(2);

        // Right pyramid
        print_hashes(row);

        // New line
        printf("\n");
    }
}

// Prints out n spaces to the terminal
void print_spaces(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" ");
    }
}

// Prints out n hashes to the terminal
void print_hashes(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}