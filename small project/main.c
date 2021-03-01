
// JAKUB PANASIUK 418362

#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#define size_begin 4

int main(void)
{
    Tree t = NULL;
	int x;
    unsigned char last_char = '\n', c;
    int size = size_begin , idx = 1, check_fun = 0, print_fun = 0;
	int del_fun = 0, add_fun = 0, command = 1, wrong = 0, ignore = 0;
    int animal = 1, forest = 1, tree = 1;
    int meta_forest = 0, meta_animal = 0, meta_tree = 0;
	char written = 0;
	unsigned char * array, * animal_arr, * tree_arr, * forest_arr;

    if ((array = malloc((size+5) * sizeof(char))) == NULL)
		end();
	if ((animal_arr = malloc((size+5) * sizeof(char))) == NULL)
		end();
	if ((tree_arr = malloc((size+5) * sizeof(char))) == NULL)
		end();
	if ((forest_arr = malloc((size+5) * sizeof(char))) == NULL)
		end();

	forest_arr[1] = '\0';
	tree_arr[1] = '\0';		
	animal_arr[1] = '\0';

    while (1) {
        x = getchar ();
		c = x;
        if (last_char == '\n' && c == '#') ignore = 1;
        last_char = c;
        if (ignore && c == '\n') {ignore = 0; continue;}
        if (ignore && x==-1) break;
		else if (ignore) continue;

        if (c != ' ' && c != '\t' && c != '\v' && c != '\f' && c != '\r' && c != '\n' && x != -1) {
			if (c < 33) wrong = 1;
			written = 1;
            array[idx] = c;
            idx++;

            if (size == idx) {
                size *= 2;
				char * copy;

				if ((copy = malloc((size+5) * sizeof(char))) == NULL)
					end();

				for (int i = 1; i < idx; i++)
					copy[i] = array[i];
				free(array);

				if ((array = malloc((size+5)*sizeof(char))) == NULL)
					end();

				for (int i = 1; i < idx; i++)
					array[i] = copy[i];

				array[idx]='\0';
				free(copy);
            }
        }
        else {
			array[idx] = '\0';
            if (idx == 1) idx = 1;
            else if (command) {

                if (idx == 4) {
                    if (array[1] == 'A' && array[2] == 'D' && array[3] == 'D') 
						add_fun = 1;
                    else if (array[1] == 'D' && array[2] == 'E' && array[3] == 'L') 
						del_fun = 1;
                }
                else if (idx == 6) {
                    if (array[1] == 'C' && array[2] == 'H' && array[3] == 'E' && array[4] == 'C' && array[5] == 'K') 
						check_fun = 1;
                    else if (array[1] == 'P' && array[2] == 'R' && array[3] == 'I' && array[4] == 'N' && array[5] == 'T') 
						print_fun = 1;
                }
                else wrong = 1;

                command = 0;
            }
            else if (forest) {
                if (idx == 2 && array[1] == '*' && check_fun)
					meta_forest = 1;
                else {
					int size = 0;

					for (int i = 1; array[i] != '\0'; i++)
						size++;

					if ((forest_arr = realloc(forest_arr, ((size)+5) * sizeof(char))) == NULL)
						end();

					for (int i = 1; i <= size; i++) 
						forest_arr[i] = array[i];
					
					forest_arr[size+1]='\0';
				}

                forest = 0;
            }
            else if (tree) {
                if (idx == 2 && array[1] == '*' && check_fun) 
					meta_tree = 1;
                else{
					int size = 0;

					for (int i = 1; array[i] != '\0'; i++)
						size++;

					if ((tree_arr = realloc(tree_arr, ((size)+5) * sizeof(char))) == NULL)
						end();

					for (int i = 1; i <= size; i++) 
						tree_arr[i] = array[i];
					
					tree_arr[size+1]='\0';
				}

                tree = 0;
            }
            else if (animal) {
                if (idx == 2 && array[1] == '*' && check_fun) 
					meta_animal = 1;
                else{
					int size = 0;

					for (int i = 1; array[i] != '\0'; i++)
						size++;

					if ((animal_arr = realloc(animal_arr, ((size)+5) * sizeof(char))) == NULL)
						end();

					for (int i = 1; i <= size; i++) 
						animal_arr[i] = array[i];

					animal_arr[size+1]='\0';
				}

                animal = 0;
            }
            else wrong = 1;

            if (c == '\n') {
				written = 0;
                if (wrong || ((check_fun || add_fun) && forest) || (print_fun && !forest && !tree && !animal)) 
					 fprintf( stderr, "ERROR\n" );
				else if (!check_fun && !add_fun && !del_fun && !print_fun) 
					idx = 0;
                else {
					if (check_fun) {
						int ans1 = check(forest_arr, tree_arr, animal_arr, &t, meta_forest, meta_tree, meta_animal);

						if (ans1 == 1) printf("YES\n");
						else if (ans1 == 0) printf("NO\n");
						else fprintf( stderr, "ERROR\n" );
					}
					else if (add_fun) 
						add (forest_arr, tree_arr, animal_arr, &t), printf("OK\n");
					else if (del_fun) 
						del (forest_arr, tree_arr, animal_arr, &t), printf("OK\n");
					else if (print_fun) 
						print (forest_arr, tree_arr, animal_arr, &t);
                }

				free(forest_arr);
				if ((forest_arr = malloc(size_begin * sizeof(char))) == NULL)
					end();
                free(tree_arr);
				if ((tree_arr = malloc(size_begin * sizeof(char))) == NULL)
					end();
                free(animal_arr);
				if ((animal_arr = malloc(size_begin * sizeof(char))) == NULL)
					end();

				forest_arr[1] = '\0';
				tree_arr[1] = '\0';
				animal_arr[1] = '\0';
				written = 0;

                forest = tree = animal = command = 1;
                wrong = check_fun = print_fun = del_fun = add_fun = 0;
                meta_animal = meta_forest = meta_tree = 0;
            }
            size = size_begin;
			free(array);

			if ((array = malloc(size_begin * sizeof(char))) == NULL)
				end();
            idx = 1;
        }

        if (x == -1){
			if (written)
				fprintf( stderr, "ERROR\n" );
			break;
		}
    }

    free(array);
    free(forest_arr);
    free(tree_arr);
    free(animal_arr);
    removeAll(&t);
}
