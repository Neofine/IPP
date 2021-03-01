
// JAKUB PANASIUK 418362

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include "functions.h"
#define size_begin 4

// konczy program zwracajac wartosc 1 
void end() {
	exit(1);
}

// wypisuje tablice podana jako argument wraz ze znakiem konca linii
//  tab - tablica wypisywana
void print_arr (unsigned char * tab) {
	int size = 0;

	for (int i = 1; tab[i] != '\0'; i++)
		size++;

    for (int i = 1; i <= size; i++)
		putchar(tab[i]);

    printf("\n");
}

// zwraca minimalna z dwoch liczb podanych jako argument
// a, b - liczby porownywane
int min (int a, int b) {
    if ( a <= b ) return a;
    return b;
}

// zwraca maksymalna z dwoch liczb podanych jako argument
// a, b - liczby porownywane
int max (int a, int b) {
    if ( a <= b ) return b;
    return a;
}

// porownuje 2 tablice podane w argumentach zwraca char '=' jezeli sa leksykograficznie rowne
// zwraca '>' jezeli pierwszy argument jest leksykograficznie wiekszy niz drugi
// zwraca '<' jezeli drugi argument jest leksykograficznie wiekszy niz pierwszy
// left, right - tablice charow
unsigned char l_lesser_r (unsigned char *left, unsigned char *right) {
	int last = 1;

    for (int i = 1; left[i] != '\0' && right[i] != '\0' ; i++) {
		last = i;
        if (left[i] < right[i])
            return '<';
        else if (left[i] > right[i])
            return '>';
    }

    if (left[last+1] == '\0' && right[last+1] == '\0') return '=';
    else if (left[last+1] == '\0') return '<';
    else return '>';
}

// czysci pamiec o calym drzewie podanym w argumencie
// Ptr - drzewo
void removeAll(Tree *Ptr) {
	Tree t = *Ptr;
	if (t != NULL) {
		removeAll(&t->left);
		removeAll(&t->right);
		removeAll(&t->sub);
		free(t->here);
		free(t);

		t = NULL;
		*Ptr = t;
	}
}

//funkcja jezeli tablicy charow nie ma na drzewie dodaje ja razem z nowym wierzcholkiem i zwraca to drzewo
//w przeciwnym wypadku zwraca to drzewo bez wprowadzania zadnych zmian
// Ptr - drzewo
// text - tablica charow
Tree addT(Tree *Ptr, unsigned char *text) {
    Tree t = *Ptr;
    if (t == NULL) {

        if ((t = (Tree)realloc(t, sizeof(struct Node))) == NULL)
			end();

		t->here = NULL;
		int size = 0;

		for (int i = 1; text[i] != '\0'; i++)
			size++;

		if ((t->here = malloc(((size)+5) * sizeof(char))) == NULL) 
			end();

		for (int i = 1; i <= size; i++) {
			t->here[i] = text[i];
		}

		t->here[size+1]='\0';
        t->left = t->right = t->sub = NULL;
        *Ptr = t;

        return t;
    }
    else {
        unsigned char outcome = l_lesser_r (text, (t->here));

        if (outcome == '<')
            return addT(&t->left, text);
        else if (outcome == '>')
            return addT(&t->right, text);
        else return t;
    }
    return NULL;
}

//funkcja przyjmuje 3 tablice charow i drzewo i dodaje je do drzewa na kazdym poziomie je jezeli
//nie byly one przedtem dodate na nim
// forest, tree, animal - tablice charow
// t - drzewo
void add (unsigned char *forest, unsigned char *tree, unsigned char *animal, Tree *t) {
    Tree forest_node = addT(t, forest);
    assert(forest_node);
    if (tree[1] != '\0') {
        Tree tree_node = addT(&(forest_node->sub), tree);
        assert(tree_node);
        if (animal[1] != '\0') {
            Tree animal_node = addT(&(tree_node->sub), animal);
            assert(animal_node);
        }
    }
}

//funkcja sprawdza czy na drzewie znajduje sie podana tablica charow
// Ptr - drzewo
// text - tablica charow
Tree checkT(Tree *Ptr, unsigned char *text) {
    Tree t = *Ptr;
    if (t == NULL) {
        return NULL;
    }
    else {
        unsigned char outcome = l_lesser_r (text, (t->here));

        if (outcome == '<')
            return checkT(&t->left, text);
        else if (outcome == '>')
            return checkT(&t->right, text);
        else return t;
    }
    return NULL;
}

// funkcja przechodzi cale aktualne drzewo i wchodzi ewentualnie do poddrzew i sprawdza na
// odpowiednim poziomie czy istnieje wierzcholek rowny szukanej tablicy charow
// Ptr - drzewo
// level - aktualny poziom zaglebienia
// text - tablica charow poszukiwana
// max_lvl - maksymalny poziom zaglebienia
int walkFT (Tree *Ptr, int level, unsigned char * text, int max_lvl) {
	Tree t = *Ptr;
	if (t != NULL) {
		if (level <= max_lvl) {
			if (walkFT(&t->right, level, text, max_lvl)) return 1;
			else if (walkFT(&t->sub, level+1, text, max_lvl)) return 1;
			else return walkFT(&t->left, level, text, max_lvl);
		}
		else {
			unsigned char outcome = l_lesser_r ( t->here, text);

			if (outcome == '=' )
				return 1;
			else if (outcome == '<') 
				return walkFT(&t->right, level, text, max_lvl);
			else if (outcome == '>')
				return walkFT(&t->left, level, text, max_lvl);
		}
  	}
	return 0;
}

// funkcja wchodzi do wierzcholkow drzewa i dla kazdego wierzcholka w ktorym sie znajduje
// sprawdza czy jest w nim tablica charow rowna drugiemu argumentowi
// jak nie to wywoluje sie dalej a jak jest to sprawdza czy w poddrzewie wierzcholka znajduje sie
// takica charow rowna trzeciemu argumentowi
// Ptr - drzewo
// tree - tablica charow 
// animal - tablica charow
int walkF (Tree *Ptr, unsigned char * tree, unsigned char * animal) {
	Tree t = *Ptr;
	if (t != NULL) {
		Tree tree_node = checkT(&(t->sub), tree);
		if (tree_node != NULL) {
			Tree animal_node = checkT(&(tree_node->sub), animal);
			if (animal_node != NULL) return 1;
		}

		if (walkF(&t->left, tree, animal)) return 1;
		else return walkF(&t->right, tree, animal);
  	}
	return 0;
}

// funkcja glowna sprawdzajaca, dla danych 3 tablic charow i 3 meta znakow sprawdza czy 
// dane drzewo las oraz zwierze zostalo dodane do programu, uwzglednia meta znaki *
// forest, tree, animal - tablice charow
// t - drzewo
// meta_forest, meta_tree, meta_animal - meta znaki odpowiadajace wpisaniu * zamiast nazwy
int check (unsigned char *forest, unsigned char *tree, unsigned char *animal, Tree *t, int meta_forest,
int meta_tree, int meta_animal) {
    if (meta_animal || (meta_tree && animal[1] == '\0') || (meta_forest && tree[1] == '\0' && animal[1] == '\0')) return -1;
	else if (meta_forest && meta_tree) {
		return walkFT(t, 1, animal, 2);
	}
	else if (meta_forest) {
		if (animal[1] == '\0'){ 
			return walkFT(t, 1, tree, 1);
		}
		else {
			return walkF(t, tree, animal);
		}
	}
	else if (meta_tree) {
		Tree forest_node = checkT(t, forest);
		return walkFT(&forest_node->sub, 2, animal, 2);
	}

	Tree forest_node = checkT(t, forest);
	if (forest_node != NULL) {
	    if (tree[1] != '\0') {
	        Tree tree_node = checkT(&(forest_node->sub), tree);
	        if (tree_node != NULL) {
	            if (animal[1] != '\0') {
	                Tree animal_node = checkT(&(tree_node->sub), animal);
	                if (animal_node != NULL) return 1;
	                return 0;
	            }
	            return 1;
	        }
	        return 0;
	    }
	    return 1;
	}
	return 0;
}

// funkcja zwraca najmniejszy wierzcholek z calego drzewa, czyli ten ktory nie ma zadnego
// wierzcholka po lewej
// Ptr - drzewo
Tree * removeMin(Tree *Ptr) {
	Tree t = *Ptr;
	if (t->left == NULL) {
		return Ptr;
	}
	else {
		return removeMin(&t->left);
	}
}

// funkcja usuwa wierzcholek rowny drugiemu argumentowi
// zawsze bedzie taki wierzcholek
// Ptr - drzewo
// text - tablica charow
void removeValue(Tree *Ptr, unsigned char *text) {
	Tree t = *Ptr;
	if (t != NULL) {
		char outcome = l_lesser_r (text,  t->here);
		if ( outcome == '<' ) {
		  removeValue(&t->left, text);
		}
		else if ( outcome == '>' ) {
		  removeValue(&t->right, text);
		}
		else {
			if (t->right == NULL) {
				*Ptr = t->left;
				removeAll(&t->sub);
				free(t->here);
				free(t);
				return;
		 	 }
		 	 else {
				removeAll(&t->sub);
				Tree * tmp = removeMin(&t->right);
				Tree t1 = *tmp;
				int size = 0;

				for (int i = 1; t1->here[i] != '\0'; i++)
					size++;

				if ((t->here = realloc(t->here, ((size)+5) * sizeof(char))) == NULL) 
					end();
				for (int i = 1; i <= size; i++) {
					t->here[i] = t1->here[i];
				}

				t->here[size+1]='\0';
				t->sub = t1->sub;
				*tmp = t1->right;
				free(t1->here);
				free(t1);
		 	 }
		}
	}
}

// funkcja usuwa podane w argumentach tabice charow na drzewie t
// forest, tree, animal - tablice charow
// t - drzewo
void del (unsigned char *forest, unsigned char *tree, unsigned char *animal, Tree *t) {
    if (forest[1] != '\0') {
        if (tree[1] != '\0') {
            Tree forest_node = checkT(t, forest);

			if (forest_node == NULL) 
				return;

            if (animal[1] != '\0') {
                Tree tree_node = checkT(&(forest_node->sub), tree);

				if (tree_node == NULL) 
					return;
                removeValue (&(tree_node->sub), animal);
            }
            else {
                removeValue (&(forest_node->sub), tree);
            }
        }
        else removeValue (t, forest);
    }
    else if (t != NULL) {
		removeAll(t);
		t = NULL;
	}
}

// funkcja wypisuje cale drzewo i jego poddrzewa
// tPtr - drzewo
void printT(Tree *Ptr) {
    Tree t = *Ptr;
    if (t != NULL) {
        printT(&t->left);
        print_arr(t->here);
        printT(&t->right);
    }
}

// funkcja wypisuje drzewa podanych na wejsciu argumentow na drzewie t
// forest, tree, animal - tablice charow
// t - drzewo
void print (unsigned char *forest, unsigned char *tree, unsigned char *animal, Tree *t) {
    if (animal[1] == '\0') {
        if (tree[1] == '\0') {
            if (forest[1] == '\0'){
                printT(t);
                return;
            }
            Tree forest_node = checkT(t, forest);
			if (forest_node != NULL)
				printT(&(forest_node->sub));
	  		return;
        }
        Tree forest_node = checkT(t, forest);
        if (forest_node != NULL) {
			Tree tree_node = checkT(&(forest_node->sub), tree);
    	    if (tree_node != NULL)
				printT(&(tree_node->sub));
		}
    }
}
