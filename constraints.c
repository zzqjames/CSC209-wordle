#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "constraints.h"

/* Create and initialize a constraints struct. 
 * Sets the fields to 0 or empty string.
 * Return a pointer to the newly created constraints struct.
 */
struct constraints *init_constraints() {
    // TODO - Remember to update return statement

    	struct constraints *c = malloc(sizeof(struct constraints));	
        for (int i=0; i < ALPHABET_SIZE; i++)
        {       
                (c->cannot_be)[i] ='0';
        }

	for (int i=0; i < WORDLEN; i++)
	{
		(c->must_be)[i][0] = '\0';
	} 	

    return c;
}

/* Update the "must_be" field at "index" to be a string 
 * containing "letter"
 * The tile at this index is green, therefore the letter at "index"
 * must be "letter"
 */
void set_green(char letter, int index, struct constraints *con) {
    assert(islower(letter));
    assert(index >= 0 && index < SIZE);
    
    // TODO
	(con->must_be)[index][0]= letter;
	(con->must_be)[index][1] = '\0';
}

/* Update "con" by adding the possible letters to the string at the must_be 
 * field for "index".
 * - index is the index of the yellow tile in the current row to be updated
 * - cur_tiles is the tiles of this row
 * - next_tiles is the tiles of the row that is one closer to the solution row
 * - word is the word in the next row (assume word is all lower case letters)
 * Assume cur_tiles and next_tiles contain valid characters ('-', 'y', 'g')
 * 
 * Add to the must_be list for this index the letters that are green in the
 * next_tiles, but not green in the cur_tiles or green or yellow in the 
 * next_tiles at index.
 * Also add letters in yellow tiles in next_tiles.
 */
void set_yellow(int index, char *cur_tiles, char *next_tiles, 
                char *word, struct constraints *con) {

    assert(index >=0 && index < SIZE);
    assert(strlen(cur_tiles) == WORDLEN);
    assert(strlen(next_tiles) == WORDLEN);
    assert(strlen(word) == WORDLEN);

    // TODO
		int count = 0;
	for (int i=0; i < SIZE-1; i++)
	{
		char temp = word[i];

		if (i != index && next_tiles[i]=='g' && cur_tiles[i]!='g')
		{
		
			(con->must_be)[index][count]=temp;
			count += 1;
			(con->must_be)[index][count]='\0';		
		}
		
		if (next_tiles[i] == 'y'&& i != index)
		{
			(con->must_be)[index][count]=temp;
                        count += 1;
			(con->must_be)[index][count]='\0';
		}
	}
}

/* Add the letters from cur_word to the cannot_be field.
 * See the comments in constraints.h for how cannot_be is structured.
 */
void add_to_cannot_be(char *cur_word, struct constraints *con) {
    assert(strlen(cur_word) <= WORDLEN);

    //TODO
	
	for (int i=0; i<strlen(cur_word); i++)
	{
		(con->cannot_be)[cur_word[i]-'a']='1';
	}

}

void print_constraints(struct constraints *c) {
    printf("cannot_be: ");

    // TODO
        	char result_1[256];
	result_1[0] = '\0'; 
	int count = 0;
	for (int i=0; i<ALPHABET_SIZE; i++)
	{
		if ((c->cannot_be)[i]=='1')
		{ 	
			result_1[count] = 'a' + i;
			count+=1;
			result_1[count] = '\0';
		}
	}
	
	printf("%s\n",result_1);
    printf("\nmust_be\n");

    // TODO
	for (int i=0; i<WORDLEN;i++)
	{
		printf("%s\n",(c->must_be)[i]);
	}
    printf("\n");
}

/* Free all dynamically allocated memory pointed to by c
 */
void free_constraints(struct constraints *c) {
    // TODO
	
	free(c);
}
