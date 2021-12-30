#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Prototypes
int grade_text(string text);
void print_grade(int grade);

int main(void)
{
    // Prompt user for text
    string text = get_string("Text: ");

    // Grades the text based on the Coleman-Liau index
    int grade = grade_text(text);

    print_grade(grade);
}

// Grades the text based on the Coleman-Liau index
int grade_text(string text)
{
    int letters = 0;
    int words = 1;
    int sentences = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (islower(text[i]) || isupper(text[i]))
        {
            letters++;
        }
        else if (text[i] == ' ')
        {
            words++;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    // Average number of letters per 100 words
    float L = (float) letters / words * 100;

    // Average number of sentences per 100 words;
    float S = (float) sentences / words * 100;

    // Coleman-Liau index formula
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    return index;
}

void print_grade(int grade)
{
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}