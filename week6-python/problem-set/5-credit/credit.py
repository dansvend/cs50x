from cs50 import get_int


def main():
    # Prompt user for credit card number
    number = get_int("Credit card number: ")

    # Validates checksum
    checksum_validated = validate_checksum(number)
    if checksum_validated:
        message = card_number_validation(number)
    else:
        message = "INVALID"

    # Print whether it is a valid America Express, MasterCard or Visa card number
    print(message)


# Validates whether the credit card number has a valid checksum
def validate_checksum(number):
    number_length = get_number_length(number)

    sum = 0
    remainder = number
    for i in range(1, number_length + 1, 1):
        if i % 2 == 0:
            sum += add_digits((remainder % 10) * 2)
        else:
            sum += remainder % 10
        remainder //= 10

    # Check whether the last digit in the sum is 0
    if sum % 10 == 0:
        return True
    else:
        return False


# Gets the number of digits of a number
def get_number_length(number):
    return len(str(number))


# Adds each individual digits in a number and returns the sum
def add_digits(number):
    sum = 0
    while number > 0:
        remainder = number % 10
        sum += remainder
        number //= 10
    return sum


# Returns AMEX, MASTERCARD or VISA if the credit card number matches
# the length and start number criterias for the respective credit card issuers.
# Returns INVALID otherwise.
def card_number_validation(number):
    is_amex = validate_amex(number)
    is_mastercard = validate_mastercard(number)
    is_visa = validate_visa(number)
    if is_amex:
        return "AMEX"
    elif is_mastercard:
        return "MASTERCARD"
    elif is_visa:
        return "VISA"
    else:
        return "INVALID"


# Checks if credit card number is 15 digits long and whether it starts with 34 or 37
def validate_amex(number):
    if ((number >= 340000000000000 and number < 350000000000000) or
            (number >= 370000000000000 and number < 380000000000000)):
        return True
    return False


# Checks if credit card number is 16 digits long and whether it starts with 51, 52, 53, 54 or 55
def validate_mastercard(number):
    if number >= 5100000000000000 and number < 5600000000000000:
        return True
    return False


# Checks if credit card number is 13 or 16 digits long and whether it starts with 4
def validate_visa(number):
    if ((number >= 4000000000000 and number < 5000000000000) or
            (number >= 4000000000000000 and number < 5000000000000000)):
        return True
    return False


main()