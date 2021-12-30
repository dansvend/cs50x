from cs50 import get_int


def main():
    # Prompt user for pyramid height between 1 and 8, inclusive
    height = get_pyramid_height()

    # Prints out a right aligned pyramid of hashes with the height from the argument to the terminal
    print_pyramid(height)


# Will prompt the user for a positive integer between 1 and 8, inclusive.
def get_pyramid_height():
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            return height


# Prints out a right aligned pyramid of hashes with the height from the argument
def print_pyramid(height):
    for row in range(1, height + 1, 1):
        spaces = height - row
        print_str(spaces, " ")

        hashes = row
        print_str(hashes, "#")
        print()


# Prints out n chars to the terminal
def print_str(n, char):
    for i in range(n):
        print(char, end="")


main()