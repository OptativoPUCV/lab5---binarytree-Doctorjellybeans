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
    int (*lower_than) (void* key1, void* key2); // interesante
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
  TreeMap* nuevoMapa = (TreeMap*)malloc(sizeof(TreeMap));
  nuevoMapa->root = NULL;
  nuevoMapa->current = NULL;
  nuevoMapa->lower_than = lower_than;
  
  return nuevoMapa;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if (tree == NULL) return;

  TreeNode* nuevoNodo = createTreeNode(key, value);
  if (tree->root == NULL){
    
    tree->root = nuevoNodo;
    tree->current = nuevoNodo;
  
  } else {

    // Partimos desde la raiz para hacer las comparaciones
    TreeNode* current = tree->root;
    while (1){
      if (is_equal(tree, key, current->pair->key)){ // si key es igual
        
        free(nuevoNodo);
        return;
      
      } else if (tree->lower_than(key,current->pair->key)) { // si key es menor
        
        if (current->left == NULL){ // espacio vacio encontrado, insertar
          current->left = nuevoNodo;
          nuevoNodo->parent = current;
          tree->current = nuevoNodo;
          return;
        } else { // seguir iterando
          current = current->left;
        }
        
      } else { // si key es mayor
        
        if (current->right == NULL){ // espacio vacio encontrado, insertar
          current->right = nuevoNodo;
          nuevoNodo->parent = current;
          tree->current = nuevoNodo;
          return;  
        } else { // seguir iterando
          current = current->right;
        }
        
      }
    }
  }
}

TreeNode * minimum(TreeNode * nodoX){
  if (nodoX == NULL) return NULL;
  
  TreeNode* temp = nodoX;
  while (temp->left != NULL){
    temp = temp->left;
  }
  return temp;
}

// para el desarrollo de esta funcion decidi utilizar uno de los codigos de mi notion personal
/*
void delete(Tree* tree, int key) {
    Tree_Node* temp = tree->root;
    Tree_Node* parent = NULL;

    // Primero buscamos el nodo a eliminar y su padre
    while(temp != NULL && temp->key != key) {
        parent = temp;
        if(key < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }

    if(temp == NULL)  // Si el nodo no se encuentra en el árbol
        return;

    // Caso 1: el nodo es una hoja
    if(temp->left == NULL && temp->right == NULL) {
        if(temp != tree->root) {
            if(parent->left == temp)
                parent->left = NULL;
            else
                parent->right = NULL;
        } else {
            tree->root = NULL;
        }

        free(temp);
    }
    // Caso 2: el nodo tiene un solo hijo
    else if(temp->left == NULL || temp->right == NULL) {
        Tree_Node* child;
        if(temp->left != NULL)
            child = temp->left;
        else
            child = temp->right;

        if(temp != tree->root) {
            if(parent->left == temp)
                parent->left = child;
            else
                parent->right = child;
        } else {
            tree->root = child;
        }

        free(temp);
    }
    // Caso 3: el nodo tiene dos hijos
    else {
        Tree_Node* successor = getMinimumNode(temp->right);
        int succKey = successor->key;
        void* succData = successor->data;
        delete(tree, succKey);  // Eliminamos el sucesor
        temp->key = succKey;
        temp->data = succData;
    }
}
*/
void removeNode(TreeMap * tree, TreeNode* node) {
  if (tree == NULL) return;

  TreeNode* temp = tree->root;
  TreeNode* parent = NULL;

  // Buscamos nodo a eliminar
  while (temp != NULL && !is_equal(tree, node->pair->key, temp->pair->key)){
    parent = temp;
    if (tree->lower_than(node->pair->key, temp->pair->key)){
      temp = temp->left;
    } else {
      temp = temp->right;
    }
  }
  
  if (temp == NULL){ // el nodo no se encontro
    return;
  }
  
  // Caso 1 Nodo hoja
  if (temp->left == NULL && temp->right == NULL){
    if(temp != tree->root) {
      if (parent->left == temp){
        parent->left = NULL;
      } else {
        parent->right = NULL;
      }
    } else {
      tree->root = NULL;
    }

    free(temp);
  
  // Caso 2 1 Hjo  
  } else if (temp->left == NULL || temp->right == NULL){
    TreeNode* hijo;
    if (temp->left != NULL){
      hijo = temp->left;
    } else {
      hijo = temp->right;
    }
    if (temp != tree->root){
      if (parent->left == temp){
        parent->left = hijo;
      } else {
        parent->right = hijo;
      }
    } else {
      tree->root = hijo;
    }

    free(temp);
  // 2 Hijos
  } else {
    TreeNode* siguiente = minimum(temp->right);
    node->pair->key = siguiente->pair->key;
    node->pair->value = siguiente->pair->value;
    removeNode(tree,siguiente);
  }
  
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  if (tree == NULL) return NULL;

  // Exploremos desde la raiz
  TreeNode* current = tree->root;
  while (current != NULL){
    
    if (is_equal(tree, key, current->pair->key)){ // si key es igual
      
      tree->current = current;
      return current->pair;
    
    } else if (tree->lower_than(key,current->pair->key)){ // si key es menor
      
      current = current->left;      
    
    } else { // si key es mayor
      
      current = current->right;
    
    }
  
  }
  // Si no se encuentra la clave retorna NULL.
  tree->current = NULL;
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
