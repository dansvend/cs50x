#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Prototypes
bool validate_argument(string argument);
string encrypt_plaintext(string text, string key);

int main(int argc, string argv[])
{
    // Returns error code 1 if the wrong number of command line arguments is used and exits the program
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Returns error code 1 and exits the program if the argument is not a valid key
    bool valid_arg = validate_argument(argv[1]);
    if (!valid_arg)
    {
        return 1;
    }
    string key = argv[1];

    // Prompt user for plaintext
    string plaintext = get_string("plaintext: ");

    // Encrypt plaintext into ciphertext. Only encrypts alphabetical characters
    string ciphertext = encrypt_plaintext(plaintext, key);

    // Print ciphertext and return code 0 to indicate that the program ran without issues
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

// Checks that the key is 26 characters long
// Checks that the key only contains alphabetic characters
// Checks that the key does not have duplicate letters
bool validate_argument(string argument)
{
    if (strlen(argument) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }

    // Array to keep track of letters in the argument to make sure there's no duplicates
    char letters[26];
    int index = 0;
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(argument[i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return false;
        }

        for (int j = 0; j < 26; j++)
        {
            if (tolower(argument[i]) == letters[j])
            {
                printf("Key must not contain repeated characters.\n");
                return false;
            }
        }
        letters[index] = tolower(argument[i]);
        index++;
    }
    return true;
}

// Encrypt plaintext into ciphertext. Only encrypts alphabetical characters
string encrypt_plaintext(string text, string key)
{
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isalpha(text[i]))
        {
            char new_char;
            if (islower(text[i]))
            {
                int alpha_index = text[i] - 'a';
                new_char = tolower(key[alpha_index]);
            }
            else
            {
                int alpha_index = text[i] - 'A';
                new_char = toupper(key[alpha_index]);
            }
            text[i] = new_char;
        }
    }
    return text;
}