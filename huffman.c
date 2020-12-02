#include "huffman.h"

Tree create_huffman_tree(List * occurrencesList){
    if(occurrencesList != NULL){
        Element * leftE = popMin(occurrencesList);
        Node * left = create_node(leftE->chara, 1, leftE->occurrences, NULL, NULL);
        Element * rightE = popMin(occurrencesList);
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

void translate_texte_with_huffman(){

    FILE* texte = fopen("texte.txt", "r");
    FILE* code = fopen("dico.txt", "r");
    FILE* encode_texte = fopen("compressed.txt", "w+");

    char letter = fgetc(texte);
    while (letter != EOF){

        char* take_info = (char*)malloc(12*sizeof(char));
        do{
            fgets(take_info, 12, code);
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
Fonction globale occurences* array_of_occurences();
1-Créé array // Facile
2. Create new structure : occurences
	char/int letter
	int occurence
3-prendre une lettre // facile
	"""Dans la fonction globale"""
4-Recherche dicotomique // difficile
	length
	if > length/2
	if < length/2
5-deux cas :
	5.1-si elle existe et bah on fait plus 1 à l'occurrence // facile
		"""intégrée dans la globale"""
		array[i]->occurence+=1
	5.2-si elle existe pas
		On créé une node avec al lettre // facile mis structure = node + int
			Parameters: letter
		5.2.1 on recréé larray en +1 // facile
			Parameters : Length array
			Return new Array
		5.2.2 et la on additione les 2 arrays : on copicolle l'array jusqua quon print le new nombre  puis on continue le print de larray de base // difficile
			parameters : Array
				     Length
				     Position letter
				     New array
			free old array !!
			return new array
6-On continue jusqua la fin du texte // Facile
	Boucle while (txt_letter != EOF)
"""On return l'array"""
