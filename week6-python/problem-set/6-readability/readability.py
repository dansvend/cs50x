from cs50 import get_string


def main():
    # Prompt user for text
    text = get_string("Text: ")

    # Grades the text based on the Coleman-Liau index
    grade = grade_text(text)

    print_grade(grade)


# Grades the text based on the Coleman-Liau index
def grade_text(text):
    letters = 0
    words = 1
    sentences = 0
    for i in range(len(text)):
        if text[i].islower() or text[i].isupper():
            letters += 1
        elif text[i] == ' ':
            words += 1
        elif text[i] == '.' or text[i] == '!' or text[i] == '?':
            sentences += 1

    # Average number of letters per 100 words
    L = letters / words * 100

    # Average number of sentences per 100 words;
    S = sentences / words * 100

    # Coleman-Liau index formula
    index = round(0.0588 * L - 0.296 * S - 15.8)
    return index


def print_grade(grade):
    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


main()