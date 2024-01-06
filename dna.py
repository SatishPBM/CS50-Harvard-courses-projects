import csv
import sys


def main():

    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")

    # read from database CSV file
    data_file = open(sys.argv[1], "r")
    reader = csv.DictReader(data_file)
    data = []
    for row in reader:
        data.append(row)
    data_file.close()

    # read from sequence file
    seq_file = open(sys.argv[2], "r")
    sequence = seq_file.read()
    seq_file.close()

    pattern_count = {}  # dict to store counts of each repeated pattern in sequence file

    i = 0
    for x in data[0]:
        found = False
        offset = 0
        found_list = []  # this will store offsets for all pattern occurences in sequence file
        if i == 0:  # skip the first column
            i = 1
            continue
        offset = sequence.find(x, offset)
        if offset != -1:
            found = True
            found_list.append(offset)
        while found == True:
            offset = offset + len(x)
            offset = sequence.find(x, offset)
            if offset == -1:
                found = False
            else:
                found_list.append(offset)

        index_prev = 0
        repeat = 0
        repeat_max = 0
        r = 0

        if len(found_list) == 1:
            pattern_count[x] = 1
        else:
            for index in found_list:
                if r == 0:
                    index_prev = index
                    repeat = 1
                    r = r + 1
                else:
                    if (index - index_prev) == len(x):  # to detect if pattern repeats (consecutively)
                        repeat = repeat + 1
                        if repeat > repeat_max:  # to track the maximum value of consecutive repeats
                            repeat_max = repeat
                    else:
                        if repeat > repeat_max:
                            repeat_max = repeat
                        repeat = 1

                    index_prev = index

        if x in pattern_count:
            if repeat_max > pattern_count[x]:
                pattern_count[x] = repeat_max
        else:
            pattern_count[x] = repeat_max

        repeat_max = 0

    data_list = []  # to create a list from data file with same format as pattern_list from pattern_count, for comparison
    pattern_list = []  # a list of just the counts from patterns_count

    data_dict = {}  # to create a dict similar to data.csv but without header file

    for value in pattern_count.values():
        pattern_list.append(value)

    for d in data:
        for value in d.values():
            if not value.isalpha():
                data_list.append(int(value))
            else:
                data_key = value

        data_dict[data_key] = data_list
        data_list = []

    matched_name = ""
    for key, values in data_dict.items():  # match the two lists (sequence counts, and data counts against each name)
        if pattern_list == values:
            matched_name = key

    if matched_name != "":
        print(matched_name)
    else:
        print("No match")


if __name__ == "__main__":
    main()