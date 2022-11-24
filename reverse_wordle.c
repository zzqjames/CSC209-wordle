#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordle.h"
#include "constraints.h"

/* Read the wordle grid and solution from fp. 
 * Return a pointer to a wordle struct.
 * See sample files for the format. Assume the input file has the correct
 * format.  In other words, the word on each is the correct length, the 
 * words are lower-case letters, and the line ending is either '\n' (Linux,
 * Mac, WSL) or '\r\n' (Windows)
 */
struct wordle *create_wordle(FILE *fp) {
    struct wordle *w = malloc(sizeof(struct wordle));
    char line[MAXLINE]; 
    w->num_rows = 0;

    while(fgets(line, MAXLINE, fp ) != NULL) {
        
        // remove the newline character(s) 
        char *ptr;
        if(((ptr = strchr(line, '\r')) != NULL) ||
           ((ptr = strchr(line, '\n')) != NULL)) {
            *ptr = '\0';
        }
        
        strncpy(w->grid[w->num_rows], line, SIZE);
        w->grid[w->num_rows][SIZE - 1] = '\0';
        w->num_rows++;
    }
    return w;
}


/* Create a solver_node and return it.
 * If con is not NULL, copy con into dynamically allocated space in the struct
 * If con is NULL set the new solver_node con field to NULL.
 * Tip: struct assignment makes copying con a one-line statements
 */
struct solver_node *create_solver_node(struct constraints *con, char *word) {

    // TODO - replace return statement when function is implemented
	struct solver_node *result = malloc(sizeof(struct solver_node));
	if (con != NULL){
        struct constraints *con1= malloc(sizeof(struct constraints));
        memcpy(con1,con, sizeof(struct constraints));
        result -> con = con1;}

    if (con == NULL)
	{
		result->con = NULL;
	}
	
	strcpy(result->word, word);
	
	result->next_sibling = NULL;
	result->child_list = NULL;	
    return result;
}

/* Return 1 if "word" matches the constraints in "con" for the wordle "w".
 * Return 0 if it does not match
 */
int match_constraints(char *word, struct constraints *con, 
struct wordle *w, int row) {

    // TODO

    // check if the word[index] is in the must_be[index] string.

     for (int i=0; i < WORDLEN; i++)  
    {  
        if (con->must_be[i][0] != '\0' && strchr((con->must_be)[i],word[i]) == NULL) //true when  word[i] not in the string  
        {  
            return 0;
        }         
    }  

    
    // If  must_be[index] is the empty string and word[index] is in the cannot_be set, this word does not match the constraints.

    for (int i=0; i < WORDLEN; i++){
          
            if ((con->must_be)[i][0] == '\0' && (con->cannot_be)[word[i]-'a']=='1')  
            {  
                return 0;  
            }  
        }    
    return 1;

}
/* remove "letter" from "word"
 * "word" remains the same if "letter" is not in "word"
 */
void remove_char(char *word, char letter) {
    char *ptr = strchr(word, letter);
    if(ptr != NULL) {
        *ptr = word[strlen(word) - 1];
        word[strlen(word) - 1] = '\0';
    }
}

/* Build a tree starting at "row" in the wordle "w". 
 * Use the "parent" constraints to set up the constraints for this node
 * of the tree
 * For each word in "dict", 
 *    - if a word matches the constraints, then 
 *        - create a copy of the constraints for the child node and update
 *          the constraints with the new information.
 *        - add the word to the child_list of the current solver node
 *        - call solve_subtree on newly created subtree
 */

void solve_subtree(int row, struct wordle *w,  struct node *dict, 
                   struct solver_node *parent) {
    if(verbose) {  
        printf("Running solve_subtree: %d, %s\n", row, parent->word);  
    }  
  
    // TODO  
                char *the_row = (w->grid)[row];  
                        for (int i=0; i < WORDLEN; i++)  
                        {  
                                if (the_row[i] == 'g')  
                                {  
                                        set_green((parent->word)[i], i, parent->con);  
                                }  
                                else if (the_row[i] == '-')  
                                {  
                                    parent->con->must_be[i][0] = '\0';}                                    
                                else{     
                                    if (row == 1){  
                                            char temp[SIZE];  
                                            for (int i = 0; i < SIZE -1; i++){  
                                                temp[i] = 'g';  
                                            }  
                                            temp[SIZE-1] = '\0';  
                                            set_yellow(i, the_row, temp,parent->word, parent->con);     
                                    }  
                                    if (row != 1){            
                                    set_yellow(i, the_row, (w->grid)[row-1],parent->word, parent->con);}     
                    }  
                      
                        }  
  
            add_to_cannot_be(parent->word, parent->con);  
  
        /*if(verbose) { 
        print_constraints(parent->con); 
        } */  
    struct node *curr = dict;  
    while (curr != NULL){  
  
        if (match_constraints(curr->word, parent->con, w, row) != 0)  
        {  
            //printf("hiiiiiiiiii\n");  
            struct solver_node *a_child = create_solver_node(parent->con, curr->word);  
  
            if (parent->child_list != NULL)  
            {  
                struct solver_node *curr1 = parent->child_list;

                int flag = 0;

                while (flag != 1){ 
                    if (curr1->next_sibling == NULL){flag = 1;}
                    else{curr1 = curr1->next_sibling;}
                }
                (curr1)->next_sibling = a_child; 
                /* printf("parent word: %s\n", parent->word); 
                printf("child word: %s\n", parent->child_list->word); 
                 printf("sibling word: %s\n", (curr1)->next_sibling ->word);*/  
            }             
            if (parent->child_list == NULL){  
                //printf("parent word: %s\n", parent->word);  
                  
                parent->child_list = a_child;  
                //printf("child word: %s\n", parent->child_list->word);  
            }  
              
            int end  = w->num_rows;  
            int new_row = row + 1;
            if (end > new_row){  
                solve_subtree(new_row, w, dict, a_child);  
            }  
                      
        }  
        curr = curr->next;         
    }  
  
    // debugging suggestion, but you can choose how to use the verbose option  
  
  
    // TODO  
  
}

/* Print to standard output all paths that are num_rows in length.
 * - node is the current node for processing
 * - path is used to hold the words on the path while traversing the tree.
 * - level is the current length of the path so far.
 * - num_rows is the full length of the paths to print
 */

void print_paths(struct solver_node *node, char **path, int level, int num_rows){


    path[level-1] = node->word;

	if (level == num_rows){
		for (int i = 0; i < level; i++)
		{ 
			printf("%s ",path[i]);
		}
		printf("\n");	
	}

    struct solver_node *curr = node->child_list;

    while(curr != NULL){
        print_paths(curr, path, level+1, num_rows);
        curr = curr->next_sibling;
    }
}

/* Free all dynamically allocated memory pointed to from w.
 */ 
void free_wordle(struct wordle *w){
    // TODO
	free(w);
}

/* Free all dynamically allocated pointed to from node
 */
void free_tree(struct solver_node *node){
    // TODO
	free(node);	
}
