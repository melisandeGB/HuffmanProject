#include "huffman.h"

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
Element_occur* list_occurence(){

    List head = NULL;
    List current = NULL;
    List new_node = NULL;


    FILE* ptr = fopen("texte.txt", "r");

    if (ptr == NULL){
        printf("error");
        return 0;
    }

    int ch;
    int char_found;

    while ( (ch = fgetc(ptr)) != EOF )
    {
        current = head;
        char_found = 0;
        while ((current != NULL) && (!char_found)){
            if (current->chara == ch){
                current->occurrences++;
                char_found = 1;
            }
            current = current->next;
        }
        if (!char_found){
            new_node = create_element(ch,1);
            new_node->next = head;
            head = new_node;
        }
    }
    fclose(ptr);
    return head;
}

Tree create_huffman_tree(List * occurrencesList){
    if(occurrencesList != NULL){
        Element_occur * leftE = popMin(occurrencesList);
        Node * left = create_node(leftE->chara, 1, leftE->occurrences, NULL, NULL);
        Element_occur * rightE = popMin(occurrencesList);
        Node * right = NULL;
        int occurrences = leftE->occurrences;
        free(leftE);
        if(rightE != NULL){
            right =  create_node(rightE->chara, 1, rightE->occurrences, NULL, NULL);
            occurrences += rightE->occurrences;
            free(rightE);
        }
        Node * tree = create_node(' ', 0, occurrences, left, right);
        while(*occurrencesList != NULL){
            leftE = popMin(occurrencesList);
            left = create_node(leftE->chara, 1, leftE->occurrences, NULL, NULL);
            free(leftE);
            tree = create_node(' ', 0, left->occurrences + tree->occurrences, left, tree);
        }
        return tree;
    }else{
        return NULL;
    }
}

void printDico(Node * tree, FILE * file, char * chars){
    if(tree->haveChara){
        if(tree->chara == '\n'){
            fprintf(file, "%c%c", '\\', 'n');
        }else{
            fprintf(file, "%c", tree->chara);
        }
        int i = 0;
        while(chars[i] != '\0'){
            fprintf(file, "%c", chars[i]);
            i++;
        }
        fprintf(file, "%c", '\n');
    }else{
        int i = 0;
        while(chars[i] != '\0'){
            i++;
        }
        chars[i] = '0';
        printDico(tree->left, file, chars);
        chars[i] = '1';
        printDico(tree->right, file, chars);
        chars[i] = '\0';
    }
}

int createDico(Tree huffmanTree){
    FILE * file = fopen("dico.txt", "w+");
    if(file == NULL){
        printf("Error in opening file\n");
        fclose(file);
        return 0;
    }
    int deapth = treeDeapth(huffmanTree);
    char * chars = (char*) malloc(deapth * sizeof(char));
    int i;
    for(i = 0; i < deapth; i++){
        chars[i] = '\0';
    }
    printDico(huffmanTree, file, chars);
    fclose(file);
    free(chars);
    return 1;
}


void translate_texte_with_huffman(int size_max){

    FILE* texte = fopen("texte.txt", "r");
    FILE* code = fopen("dico.txt", "r");
    FILE* encode_texte = fopen("compressed.txt", "w+");
    char letter = fgetc(texte);
    while (letter != EOF){

        char* take_info = (char*)malloc((size_max+3)*sizeof(char));
        do{
            fgets(take_info, size_max+3, code);
        }while(letter != take_info[0] && !(letter == '\n' && take_info[1] == 'n'));
        rewind(code);

        int i = 1;
        if(letter == '\n'){
            i = 2;
        }
        while(take_info[i] == '0' || take_info[i] == '1'){
            fprintf(encode_texte, "%c", take_info[i]);
            i++;
        }
        free(take_info);
        letter = fgetc(texte);
    }

    fclose(code);
    fclose(texte);
    fclose(encode_texte);
}


void decompress_text_with_huffman(Tree huffmantree){
    FILE* encoded_texte = fopen("compressed.txt", "r");
    FILE* texte = fopen("decompressed.txt", "w+");
    if(encoded_texte == NULL){
        printf("Error in opening file\n");
        fclose(encoded_texte);
        fclose(texte);
    }
    if(texte == NULL){
        printf("Error in opening file\n");
        fclose(encoded_texte);
        fclose(texte);
    }
    else{
        int number = 0;
        while(number!= EOF){
            Node* temp = huffmantree;
            while(temp->haveChara!=1 && number!=EOF){
                number=fgetc(encoded_texte);
                if(number == 49){
                    temp = temp->right;
                }
                if(number == 48){
                    temp = temp->left;
                }
            }
            fprintf(texte,"%c",temp->chara);
            temp = huffmantree;
        }
    }
    fclose(encoded_texte);
    fclose(texte);
}

void compress_file_with_huffman(){
    int question=0;
    printf("Do you want to compress your file texte.txt ? (Enter 1 if yes, 0 if no)");
    scanf("%d",&question);
    if (question==1){
        Element_occur* El = list_occurence();
        Tree huffmantree = create_huffman_tree(&El);
        createDico(huffmantree);
        translate_texte_with_huffman(treeDeapth(huffmantree));
        question=0;
        printf("Your compress has suceeded\nDo you want to decompress it too ? (Enter 1 if yes, 0 if no)");
        scanf("%d",&question);
        if (question==1){
            decompress_text_with_huffman(huffmantree);
        }
    }
    printf("Bye");
}


