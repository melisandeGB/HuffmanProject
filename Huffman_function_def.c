#include <stdio.h>
#include <stdlib.h>
#include "Huffman_function_head.h"

//A

int* letter_to_binary(int letter)
{
    int* binary = (int*) malloc(8*sizeof(int));
    binary[0] = 0;
    for (int i = 7; i >= 1; i--)
    {
        binary[i] = letter%2;
        letter = letter/2;
    }
    return binary;
}

void print_in_file(char* p_file, int* binary)
{
    FILE* bin;
    bin = fopen(p_file, "w+");
    if (bin == NULL)
    {
        printf("Error in opening file.\n");
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            fprintf(bin, "%d", binary[i]);
        }
    }
    fclose(bin);
}

void translation_file_to_binary(char* input_file, char* output_file)
{
    FILE *bin, *file;
    file = fopen(input_file, "r+");
    bin = fopen(output_file, "w+");
    char letters;
    if (file == NULL)
    {
        printf("Error in opening file\n");
    }
    else
    {
        while (!feof(file))
        {
            letters = fgetc(file);
            int letter_in_ascii = (int)letters;
            if (letter_in_ascii != -1)
            {
                int* binary = letter_to_binary(letter_in_ascii);
                print_in_file(output_file, binary);
            }
        }

    }
    fclose(bin);
    fclose(file);
}

//B

int nb_character_txt_file(char* txt_name)
{
    int count = 0;
    char letter;
    FILE *file;
    file = fopen(txt_name, "r");
    if (file == NULL)
    {
        printf("Error in opening file\n");
        return 0;
    }
    while(!feof(file))
    {
        letter = fgetc(file);
        count++;
    }
    fclose(file);
    return count-1; //Does not include spaces and returns.
}

//J













