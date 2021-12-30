#include <cs50.h>
#include <stdio.h>

// Prototypes
int get_pyramid_height(void);
void print_pyramid(int height);
void print_spaces(int n);
void print_hashes(int n);

int main(void)
{
    // Prompt user for pyramid height between 1 and 8, inclusive
    int height = get_pyramid_height();

    // Prints out a right aligned pyramid of hashes with the height from the argument to the terminal
    print_pyramid(height);
}

// Will prompt the user for a positive integer between 1 and 8, inclusive.
int get_pyramid_height(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    return height;
}

// Prints out a right aligned pyramid of hashes with the height from the argument
void print_pyramid(int height)
{
    for (int row = 1; row <= height; row++)
    {
        int spaces = height - row;
        print_spaces(spaces);

        int hashes = row;
        print_hashes(hashes);
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