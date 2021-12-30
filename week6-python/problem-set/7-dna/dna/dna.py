import csv
import sys


def main():
    # Check for correct number of command-line arguments
    if not len(sys.argv) == 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Load all STR patterns into memory
    with open(sys.argv[1], "r") as file:
        reader = csv.reader(file)
        for row in reader:
            strs = row
            strs.pop(0)
            break

    # Load DNA database into memory
    database = []
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            database.append(row)

    # Read DNA sequence into memory
    with open(sys.argv[2], "r") as file:
        sequence = file.read()

    # Analyze DNA sequence and record highest consecutive STR repeats
    repeats = {}
    for str in strs:
        repeats[str] = max_str_repeats(sequence, str)

    # Compare analysis of DNA with the provided database
    matched_strs = 0
    for person in database:
        for str in strs:
            if int(person[str]) == repeats[str]:
                matched_strs += 1
        if matched_strs == len(strs):
            print(person["name"])
            sys.exit(0)
        matched_strs = 0
    print("No match")
    sys.exit(0)


# Returns the maximum number of repeats of a STR in a DNA sequence
def max_str_repeats(sequence, str):
    len_sequence = len(sequence)
    len_str = len(str)
    repeats = []
    consecutive = 0
    for i in range(len_sequence):
        if sequence[i:i + len_str] == str:
            consecutive += 1
            for j in range(i + len_str, len_sequence, len_str):
                if sequence[j:j + len_str] == str:
                    consecutive += 1
                else:
                    break
        repeats.append(consecutive)
        consecutive = 0
    return max(repeats)


main()