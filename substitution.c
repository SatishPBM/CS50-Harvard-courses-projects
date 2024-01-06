#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

string ALPHA = "abcdefghijklmnopqrstuvwxyz";


int main(int argc, string argv[])
{
    // check that input contains one and only one command line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else
    {

        string key = argv[1];
        int key_len = strlen(key);

        if (key_len != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }

        // checks if key contains only alphabets
        for (int i = 0; i < key_len; i++)
        {
            if (isalpha(key[i]) == 0)
            {
                printf("Key must contain only alphabets.\n");
                return 1;
            }
        }

        for (int i = 0; i < 26; i++) // loop thru all 26 alphabets
        {
            int char_found = 0;

            for (int j = 0; j < key_len; j++)
            {
                if (ALPHA[i] == tolower(key[j]))  // check if each alphabet is in the Key.
                {
                    char_found = 1;
                    break;
                }
            }

            if (char_found == 0) // if an alphabet not found in Key
            {
                printf("Key does not contain each alphabet exactly once.\n");
                return 1;
            }

        }

        // prompt for plaintext
        string plaintext = get_string("plaintext: ");

        int text_len = strlen(plaintext);

        string ciphertext;

        printf("ciphertext: ");

        for (int k = 0; k < text_len; k++)
        {

            int offset = 0;

            if (islower(plaintext[k]) > 0)
            {
                offset = (int) plaintext[k] - 97; // for example, if 'A', subtract 97 from its ASCII value to get offset = 0
                printf("%c", tolower(key[offset]));
            }
            else if (isupper(plaintext[k]) > 0)
            {
                offset = (int) plaintext[k] - 65; // // for example, if 'a', subtract 65 from its ASCII value to get offset = 0
                printf("%c", toupper(key[offset]));;

            }
            else
            {
                printf("%c", plaintext[k]); //no change if not alphabetic
            }

        }

    }

    printf("\n");
    return 0;
}