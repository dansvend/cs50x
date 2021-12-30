#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

// Prototypes
bool validate_argument(string argument);
string encrypt_plaintext(string text, int key);

int main(int argc, string argv[])
{
    // Returns error code 1 if the wrong number of command line arguments is used and exits the program
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Checks whether every character in the command line argument is a decimal digit
    // Otherwise returns error code 1 and exits the program
    bool valid_arg = validate_argument(argv[1]);
    if (!valid_arg)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Converts the argument from a string to an integer
    int key = atoi(argv[1]);

    // Prompt user for plaintext
    string plaintext = get_string("plaintext: ");

    // Encrypt plaintext into ciphertext. Only encrypts alphabetical characters
    string ciphertext = encrypt_plaintext(plaintext, key);

    // Print ciphertext and return code 0 to indicate that the program ran without issues
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

// Checks whether every character in the command line argument is a decimal digit
bool validate_argument(string argument)
{
    for (int i = 0, len = strlen(argument); i < len; i++)
    {
        if (!iswdigit(argument[i]))
        {
            return false;
        }
    }
    return true;
}

// Encrypt plaintext into ciphertext. Only encrypts alphabetical characters
string encrypt_plaintext(string text, int key)
{
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isalpha(text[i]))
        {
            char new_char;
            if (islower(text[i]))
            {
                int alpha_index = text[i] - 'a';
                int cipher_index = (alpha_index + key) % 26;
                new_char = 'a' + cipher_index;
            }
            else
            {
                int alpha_index = text[i] - 'A';
                int cipher_index = (alpha_index + key) % 26;
                new_char = 'A' + cipher_index;
            }
            text[i] = new_char;
        }
    }
    return text;
}