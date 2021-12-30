#include <cs50.h>
#include <stdio.h>

// Prototypes
int validate_checksum(long number);
int get_number_length(long number);
int add_digits(int number);
string card_number_validation(long number);
int validate_amex(long number);
int validate_mastercard(long number);
int validate_visa(long number);

int main(void)
{
    // Prompt user for credit card number
    long number = get_long("Credit card number: ");

    // Validates checksum
    string message;
    int checksum_validated = validate_checksum(number);
    if (checksum_validated)
    {
        message = card_number_validation(number);
    }
    else
    {
        message = "INVALID";
    }

    // Print whether it is a valid America Express, MasterCard or Visa card number
    printf("%s\n", message);
}

// Validates whether the credit card number has a valid checksum
int validate_checksum(long number)
{
    int number_length = get_number_length(number);

    int sum = 0;
    long remaining_number = number;
    for (int i = 1; i <= number_length; i++)
    {
        if (i % 2 == 0)
        {
            sum += add_digits((remaining_number % 10) * 2);
        }
        else
        {
            sum += remaining_number % 10;
        }
        remaining_number /= 10;
    }

    // Check whether the last digit in the sum is 0
    if (sum % 10 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Gets the number of digits of a number
int get_number_length(long number)
{
    int length = 0;
    long remaining = number;
    while (remaining > 0)
    {
        remaining /= 10;
        length++;

    }
    return length;
}

// Adds each individual digits in a number and returns the sum
int add_digits(int number)
{
    int length = get_number_length(number);
    int sum = 0;
    int remaining = number;
    for (int i = 0; i < length; i++)
    {
        sum += remaining % 10;
        remaining /= 10;
    }
    return sum;
}

// Returns AMEX, MASTERCARD or VISA if the credit card number matches
// the length and start number criterias for the respective credit card issuers.
// Returns INVALID otherwise.
string card_number_validation(long number)
{
    int is_amex = validate_amex(number);
    int is_mastercard = validate_mastercard(number);
    int is_visa = validate_visa(number);
    if (is_amex)
    {
        return "AMEX";
    }
    else if (is_mastercard)
    {
        return "MASTERCARD";
    }
    else if (is_visa)
    {
        return "VISA";
    }
    else
    {
        return "INVALID";
    }
}

// Checks if credit card number is 15 digits long and whether it starts with 34 og 37
int validate_amex(long number)
{
    if ((number > 340000000000000 && number < 350000000000000) ||
        (number > 370000000000000 && number < 380000000000000))
    {
        return 1;
    }
    return 0;
}

// Checks if credit card number is 16 digits long and whether it starts with 51, 52, 53, 54 or 55
int validate_mastercard(long number)
{
    if (number > 5100000000000000 && number < 5600000000000000)
    {
        return 1;
    }
    return 0;
}

// Checks if credit card number is 13 or 16 digits long and whether it starts with 4
int validate_visa(long number)
{
    if ((number > 4000000000000 && number < 5000000000000) ||
        (number > 4000000000000000 && number < 5000000000000000))
    {
        return 1;
    }
    return 0;
}