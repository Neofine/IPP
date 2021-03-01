
// JAKUB PANASIUK

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef struct Node* Tree;

struct Node {
  unsigned char * here;
  Tree left, right, sub;
};

void end(void);

void print_arr (unsigned char * tab);

int min (int a, int b);

int max (int a, int b);

unsigned char l_lesser_r (unsigned char *left, unsigned char *right);

extern void removeAll(Tree *t);

Tree addT(Tree *Ptr, unsigned char *text);

extern void add (unsigned char *forest, unsigned char *tree, unsigned char *animal, Tree *t);

Tree checkT(Tree *Ptr, unsigned char *text);

extern int check (unsigned char *forest, unsigned char *tree, unsigned char *animal, Tree *t, int meta_forest,
int meta_tree, int meta_animal);

int walkFT (Tree *Ptr, int level, unsigned char * text, int max_lvl);

int walkF (Tree *Ptr, unsigned char * text1, unsigned char * text2);

void removeSub(Tree t);

Tree * removeMin(Tree *Ptr);

extern void del (unsigned char *forest, unsigned char *tree, unsigned char *animal, Tree *t);

void printT(Tree *Ptr);

extern void print (unsigned char *forest, unsigned char *tree, unsigned char *animal, Tree *t);

#endif /* FUNCTIONS_H */
