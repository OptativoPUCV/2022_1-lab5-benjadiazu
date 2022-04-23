#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *aux = (TreeMap*)malloc(sizeof(TreeMap));
    aux->root = NULL;
    aux->current = NULL;
    aux->lower_than = lower_than;
    return aux;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode *aux;
    searchTreeMap(tree,key);
    aux = tree->current;
    TreeNode *nuevoNodo = createTreeNode(key,value);
  
    if(aux != NULL){
        if(tree->lower_than(key,aux->pair->key) == 1){
            aux->left = nuevoNodo;
            nuevoNodo->parent = aux;
        }
        else if (tree->lower_than(key,aux->pair->key) == 1){
            aux->right = nuevoNodo;
            nuevoNodo->parent = aux;
        }
      }
      else{
          tree->root = nuevoNodo;
      }
    tree->current = nuevoNodo;
}

TreeNode * minimum(TreeNode * x){
    if (x != NULL){
        while (x->left != NULL){
            x = x->left;
        }
        return x;
    }
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode *borrar;
    TreeNode *minimo;
    TreeNode *subArbol;
    borrar = node;
    if (borrar != NULL){
        //caso sin hijos
        if (borrar->left == NULL && borrar->right == NULL){
            if (borrar == borrar->parent->left){
                borrar->parent->left = NULL;
            }
            else{
                borrar->parent->right = NULL;
            }
            free(borrar);
        }
        //caso 1 hijo
        else if ((borrar->left != NULL && borrar->right == NULL) || (borrar->right != NULL && borrar->left == NULL)){
            if (borrar==borrar->parent->left){
                if (borrar->left != NULL){
                    borrar->parent->left = borrar->left;
                    borrar->left->parent = borrar->parent;
                }
                else{
                    borrar->parent->left = borrar->right;
                    borrar->right->parent = borrar->parent;
                }
            }
            else{
                //parent tiene hijo a la derecha
                if (borrar->left != NULL){
                    borrar->parent->right = borrar->left;
                    borrar->left->parent = borrar->parent;
                }
                else{
                    borrar->parent->right = borrar->right;
                    borrar->right->parent = borrar->parent;
                }
            }
            free(borrar);
        }
        //caso 2 hijos
        else{
            subArbol = borrar->right;
            minimo = minimum(subArbol);
            borrar->pair->key = minimo->pair->key;
            borrar->pair->value = minimo->pair->value;
            removeNode(tree,minimo);
        }
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux = tree->root;
    while (aux != NULL){
        tree->current = aux;
        if (tree->lower_than(key,aux->pair->key) == 1){
            aux = aux->left;
        }
        else if (tree->lower_than(aux->pair->key,key) == 1){
            aux = aux->right;
        }
        else{
            return aux->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    
    TreeNode *aux = tree->root;
    if (aux == NULL) return NULL;
  
    while (aux->left != NULL){
        aux = aux->left;
    }
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *aux = tree->current;
    if (tree->current->right != NULL){
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }
    else{
        while (tree->current != NULL){
            while (tree->lower_than(aux->parent->pair->key,tree->current->pair->key) == 1){
                aux = aux->parent->parent;
            }
            tree->current = aux;
            return tree->current->pair;
        }
    }
    return NULL;
}
