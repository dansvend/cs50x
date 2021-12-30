#include <cs50.h>
#include <stdio.h>
#include <math.h>

// Prototypes
float get_change(void);
int calculate_coins(float dollars);

int main(void)
{
    // Prompt user for change owed in dollars
    float dollars = get_change();

    // Calculates the least number of coins to dispense the change from the argument of dollars owed
    int coins = calculate_coins(dollars);

    // Prints out the least number of coins to dispense the change
    printf("%i\n", coins);
}

// Prompt user for change owed in dollars
float get_change(void)
{
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);
    return dollars;
}

// Calculates the least number of coins to dispense the change from the argument of dollars owed
int calculate_coins(float dollars)
{
    int coins = 0;
    int cents = round(dollars * 100);
    while (cents > 0)
    {
        if (cents >= 25)
        {
            cents -= 25;
        }
        else if (cents >= 10)
        {
            cents -= 10;
        }
        else if (cents >= 5)
        {
            cents -= 5;
        }
        else
        {
            cents -= 1;
        }
        coins++;
    }
    return coins;
}