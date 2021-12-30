from cs50 import get_int


def main():
    # Prompt user for pyramid height between 1 and 8, inclusive
    height = get_pyramid_height()

    # Prints out a left aligned pyramid, a gap and a right aligned pyramid of hashes with the height from the argument to the terminal
    print_pyramids(height)


# Will prompt the user for a positive integer between 1 and 8, inclusive.
def get_pyramid_height():
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            return height


# Prints out a left aligned pyramid, a gap and a right aligned pyramid of hashes with the height from the argument to the terminal
def print_pyramids(height):
    for row in range(1, height + 1, 1):
        spaces = height - row
        # Left pyramid
        print_str(spaces, " ")
        print_str(row, "#")

        # Gap
        print_str(2, " ")

        # Right pyramid
        print_str(row, "#")

        # New line
        print()


# Prints out n chars to the terminal
def print_str(n, char):
    for i in range(n):
        print(char, end="")


main()