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
  if (tree == NULL || tree->root == NULL || node == NULL) return;
  
  if (node == NULL){ // el nodo no se encontro
    return;
  }
  
  // Caso 1 Nodo hoja
  if (node->left == NULL && node->right == NULL){
    if(node->parent != NULL) {
      if (node->parent->left == node){
        node->parent->left = NULL;
      } else if (node->parent->right == node){
        node->parent->right = NULL;
      }
    } else {
      tree->root = NULL;
    }

    free(node->pair);
    free(node);
  
  // Caso 2 1 Hijo  
  } else if (node->left == NULL || node->right == NULL){
    TreeNode* hijo = (node->left != NULL) ? node->left : node->right;
    if (node->parent != NULL){
      if (node->parent->left == node){
        node->parent->left = hijo;
      } else if (node->parent->right == node){
        node->parent->right = hijo;
      }
      hijo->parent = node->parent;
    } else {
      tree->root = hijo;
      hijo->parent = NULL;
    }
    free(node->pair);
    free(node);
    
  // 2 Hijos
  } else {
    printf("key: %d\n", *((int*)node->pair->key));
    TreeNode* min = minimum(node->right);
    node->pair->key = min->pair->key;
    node->pair->value = min->pair->value;
    removeNode(tree, min);
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
  if (tree == NULL || tree->root == NULL) {
    return NULL;
  }

  TreeNode* current = tree->root;
  TreeNode* ub_node = NULL; // nodo auxiliar

  while (current != NULL){
    if (tree->lower_than(key, current->pair->key)){
      ub_node = current;
      current = current->left;
    } else if (tree->lower_than(current->pair->key,key)){
      current = current->right;
    } else {
      return current->pair;
    }
  } 
  
  if (ub_node != NULL){
    return ub_node->pair;
  }

  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
  if (tree == NULL || tree->root == NULL) return NULL;

  tree->current = minimum(tree->root);
  if (tree->current != NULL){
    return tree->current->pair;
  }
  return NULL;
}

// una funcion adicional para depurar codigo 
void printPair(Pair* pair) {
    // Desreferencia los punteros para acceder a los valores
    int* key = (int*)(pair->key);     // Suponiendo que las claves son enteros
    int* value = (int*)(pair->value); // Suponiendo que los valores son enteros

    // Imprime los valores
    printf("Key: %d, Value: %d\n", *key, *value);
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) {
        return NULL; // Árbol no inicializado o current es NULL
    }

    TreeNode* current = tree->current;

    // Si el nodo actual tiene un subárbol derecho, busca el nodo mínimo en ese subárbol
    if (current->right != NULL) {
        current = current->right;
        while (current->left != NULL) {
            current = current->left;
        }
        tree->current = current; // Actualiza el puntero current del árbol
        return current->pair;
    }

    // Si el nodo actual no tiene subárbol derecho, busca el ancestro más cercano
    while (current->parent != NULL && current == current->parent->right) {
        current = current->parent;
    }

    current = current->parent;
    tree->current = current; // Actualiza el puntero current del árbol

    if (current != NULL) {
        return current->pair;
    } else {
        return NULL; // Llegamos al final del árbol
    }
}
