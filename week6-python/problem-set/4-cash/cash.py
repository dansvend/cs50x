from cs50 import get_float


def main():
    # Prompt user for change owed in dollars
    dollars = get_change()

    # Calculates the least number of coins to dispense the change from the argument of dollars owed
    coins = calculate_coins(dollars)

    # Prints out the least number of coins to dispense the change
    print(coins)


# Prompt user for change owed in dollars
def get_change():
    while True:
        dollars = get_float("Change owed: ")
        if dollars >= 0:
            return dollars


# Calculates the least number of coins to dispense the change from the argument of dollars owed
def calculate_coins(dollars):
    coins = 0
    cents = round(dollars * 100)
    while cents > 0:
        if cents >= 25:
            cents -= 25
        elif cents >= 10:
            cents -= 10
        elif cents >= 5:
            cents -= 5
        else:
            cents -= 1
        coins += 1
    return coins


main()