#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main(void)
{

    // Prompt for input string
    string text = get_string("Text: ");

    int count_letters = 0;
    int text_len = strlen(text);
    int count_words = 0;
    int alpha_found = 0;
    int count_sentences = 0;

    for (int i = 0; i < text_len; i++)
    {
        // count the number of letters and words in input string
        if (isalpha(text[i]) != 0)
        {
            count_letters = count_letters + 1;
            alpha_found = 1; // is alphabetic
        }
        else
        {
            if (alpha_found == 1) // if preceding char is alphabetic
            {
                if ((text[i] != '-') && (text[i] != '\''))
                {
                    count_words = count_words + 1;
                    alpha_found = 0;
                }

                if ((text[i] == '.') || (text[i] == '?') || (text[i] == '!')) // end of sentence markers . ? and !
                {
                    count_sentences = count_sentences + 1;
                }
            }
        }

    }

    // calculate Coleman-Liau index
    float index = 0.0588 * 100 * count_letters / count_words - 0.296 * 100 * count_sentences / count_words - 15.8;

    // display Grade based on Coleman-Liau index value
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (round(index) > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.f\n", round(index));
    }
}