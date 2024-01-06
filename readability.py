from cs50 import get_string

text = get_string("Text: ")

# initialize counts
letters_count = 0
words_count = 0
sentences_count = 0

for c in text:
    if c.isalpha():  # to count letters
        letters_count = letters_count + 1
        is_word = True  # to indicate it is part of a word, used to count words below

    if c in (' ', ',', '.', '!', '?') and (is_word == True):  # to count words
        words_count = words_count + 1
        is_word = False

    if c in ('.', '!', '?'):  # to count letters
        sentences_count = sentences_count + 1

# calculate Coleman-Liau index
index = 0.0588 * 100 * letters_count / words_count - 0.296 * 100 * sentences_count / words_count - 15.8

# display Grade based on Coleman-Liau index value
if index < 1:
    print("Before Grade 1")
elif round(index) > 15:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")
