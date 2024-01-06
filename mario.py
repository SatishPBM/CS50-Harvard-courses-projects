from cs50 import get_int

valid_height = False

while not valid_height:
    height = get_int("Height: ")
    if height in range(1, 9):
        valid_height = True

# Start building the pyramid

for i in range(1, height + 1, 1):

    # Build the preceding spaces
    for j in range(height - i + 1, 1, -1):
        print(" ", end="")

    # print left pyramid
    for k in range(1, i + 1, 1):
        print("#", end="")

    # print two spaces in the middle
    print("  ", end="")

    # print right pyramid
    for k in range(1, i + 1, 1):
        print("#", end="")

    print("")

