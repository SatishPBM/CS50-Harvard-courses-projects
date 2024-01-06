from cs50 import get_int


# prompt for card number until it gets a numeric value
card = get_int("Number: ")

temp_card = card
n = 0
dig = 0
checksum = 0
card_type = "INVALID"

while temp_card > 0:

    # validate card using Luhns algorithm. The logic is to  divide the card number
    # by 10 successively and to use remainder to get each digit right to left.

    n = n + 1
    dig = temp_card % 10
    temp_card = int(temp_card / 10)

    if (n % 2) == 0:
        if (2 * dig) > 9:
            checksum = checksum + ((2 * dig) % 10 + int((2 * dig) / 10))
        else:
            checksum = checksum + 2 * dig
    else:
        checksum = checksum + dig


if (checksum % 10) == 0:

    card_str = str(card)

    # check for Amex
    if len(card_str) == 15:
        if int(card_str[:2]) in (34, 37):
            card_type = "AMEX"

    # Check for Mastercard
    if len(card_str) == 16:
        if int(card_str[0:2]) in range(51, 56, 1):
            card_type = "MASTERCARD"

    # check for Visa - 13 digits
    if len(card_str) == 13:
        if int(card_str[0]) == 4:
            card_type = "VISA"

    # check for Visa - 16 digits
    if len(card_str) == 16:
        if int(card_str[0]) == 4:
            card_type = "VISA"

print(card_type)
