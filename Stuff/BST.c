#include <stdio.h>
#include <stdlib.h>

/*
    Funzionante, per trasportarlo devo aggiungere i nuovi attributi alla struct,
    aggiuranare con gli altri attributi nella delete,aggiungere attributi in crea_node,
    fare in modo che vengano inseriti i dati presi dal file.
    Tabella hash per cars????
*/
typedef struct nodo{
    unsigned int dist;
    // short int num_car;
    // unsigned int car[512];
    struct nodo *left,*right,*p;
}stazione;

stazione *crea_node(unsigned int far);
stazione *succ(stazione *x);
stazione *insert(stazione *root,stazione *z);
stazione *search(stazione *root,unsigned int key);
stazione *max(stazione *root);
stazione *min(stazione *root);
stazione *delete(stazione *root,stazione *z);
void stampa(stazione *root);

int main(){
    stazione *primo,*secondo,*terzo,*q,*c,*s,*root;
    root = (stazione *) malloc (sizeof(stazione));
    root=NULL;
    primo = crea_node(20);
    secondo = crea_node(30);     //Creao stazione prima di inserirla
    terzo = crea_node(10);
    q = crea_node(5);
    c = crea_node(25);
    s = crea_node(12);

    root = insert(root,primo);
    root = insert(root,secondo);
    root = insert(root,terzo);
    root = insert(root,q);
    root = insert(root,c);
    root = insert(root,s);

    stampa(root);
    printf(" \n prima di delete \n");

    root = delete(root,primo);
    stampa(root);
    printf("\n");
    return 0;
}

stazione *crea_node(unsigned int far){
    stazione *temp = (stazione *) malloc (sizeof(stazione));
    temp->dist = far;
    temp->left = temp->right = temp->p = NULL;
    return temp;
}

stazione *insert(stazione *root,stazione *z){
    stazione *y=NULL,*x=NULL,*radice=NULL;
    x=root;
    radice=root;
    while(x!=NULL){
        y=x;
        if(z->dist<x->dist)                                    //Scorro il BST
            x=x->left;
        else
            x=x->right;
    }
    z->p = y;
    if(y==NULL){
        radice = z;
    }
    else{
        if(z->dist < y->dist)
            y->left = z;                    // lo posiziono nella giusta parte
        else
            y->right=z;
    }
    return radice;
}


void stampa(stazione *root){
    if(root!=NULL){
        printf("%d ",root->dist);
        stampa(root->left);
        stampa(root->right);
    }
    else
        printf("- ");
}

stazione *search(stazione *root,unsigned int key){
    if(root == NULL || root->dist == key)
        return root;
    if(root->dist < key)
        return search(root->left,key);
    else
        return search(root->right,key);
}

stazione *max(stazione *root){
    while(root->right != NULL)
        root = root->right;
    return root;
}

stazione *min(stazione *root){
    while(root->left != NULL)
        root = root->left;
    return root;
}

stazione *succ(stazione *x){
    stazione *y;
    if(x->right != NULL)
        return min(x->right);
    y = x->p;
    while(y!=NULL && x == y->right){
        x = y;
        y = y->p;
    }
    return y;
}

stazione *delete(stazione *root,stazione *z){
//Possiamo avere 3 casi: se il nodo da eliminare a 1,2 o 0 figli
    stazione *y,*x;
    if(z->left == NULL || z->right == NULL)
        y = z;
    else
        y = succ(z);
    if(y->left != NULL)
        x = y->left;
    else
        x = y->right;
    if(x!=NULL)
        x->p = y->p;
    if(y->p == NULL)
        root = x;
    else{
        if(y == y->p->left)
            y->p->left = x;
        else
            y->p->right = x;
    }
    if(y!=z)
        z->dist = y->dist; // e eventualmete altri dati
    free(y);
    return root;
}
