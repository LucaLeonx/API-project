#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define cap 512

typedef struct tabella{
    int el[512];
    int count;
    int max;
}Htable;


typedef struct nodo{
    int dist;
    char color;
    Htable *tab;
    struct nodo *left,*right,*p;
}stazione;

typedef struct node{
    stazione *station;
    struct node *prox,*pi;
}lista;


typedef struct queue{
    lista *l;
    struct queue *next; //next va verso destra ->
}el_queue;

el_queue *head = NULL,*tail = NULL;

void enqueue(lista *s){                              //aggiungo l'elemento in coda
    el_queue *new = (el_queue *) malloc(sizeof(el_queue));
    new->l = s;
    new->next = NULL;
    if(tail == NULL)
        head = tail = new;
    else{
        tail->next = new;
        tail = tail->next;
    }

}

lista *dequeue(){                                            // tolgo elemento in testa
    if(head != NULL){
        if(tail == head)
            tail = NULL;
        lista *s;
        el_queue *t;
        t = head;
        s = t->l;
        head = head->next;
        free(t);
        return s;
    }
    else{
        head = tail = NULL;
        return NULL;
    }
}


int print_path(lista *i,lista *f,FILE *fo){
    if(f == i)
        return 1;
    else{
        fprintf(fo,"%d ",f->station->dist);
        print_path(i,f->pi,fo);
    }
    return 1;
}

int print_path_reverse(lista *i,lista *f,FILE *fo){
    if(f == i)
        return 1;
    else{
        print_path_reverse(i,f->pi,fo);
        fprintf(fo," %d",f->station->dist);
    }
    return 1;
}


Htable *crea_tabella(){
    Htable *t;
    t = (Htable *) malloc(sizeof(Htable));
    int i;
    t->count = 0;
    t->max = 0;
    for(i=0;i<512;i++)
        t->el[i] = -2;         // equivale a mettere tutti gli elementi a NULL
    return t;

}

int insert_hash(int key,Htable *map){
    int j=0,i;
    map->count++;
    for(i=0;i<cap;i++){
        j = (key+i) % 512;
        if(map->el[j] == -2 || map->el[j] == -1){          // -1 equivale a DELETE; -2 equivale a NULL
            map->el[j] = key;
            if(key > map->max)
                map->max = key;
            return 1;
        }
    }
    map->count--;
    return 0;
}

void stampa_hash(Htable *map,FILE *f){
    int i;
    for(i=0;i<20;i++){
        fprintf(f," %d",map->el[i]);
    }
    fprintf(f,"\n %d \n",map->max);
}

int search_hash(Htable *map,int key){
    int i = 0,j;
    do{
        j = (key+i) % 512;
        if(map->el[j] == key)
            return j;
        else
            i++;
    }while( map->el[j] != -2 && i <512);
    return -1;
}

void new_table_max(Htable *map){
    int i;
    map->max = 0;
    for(i=0;i<cap;i++){
        if(map->max < map->el[i])
            map->max = map->el[i];
    }
}

void delete_hash(int pos,Htable *map){
    int key = 0;
    if(pos!= -1){
        key = map->el[pos];
        map->el[pos] = -1;
    }
    map->count--;
    if(key == map->max)
        new_table_max(map);
}

stazione *crea_node(int far){
    stazione *temp = (stazione *) malloc (sizeof(stazione));
    temp->dist = far;
    temp->color = 'w';
	temp->tab = crea_tabella();
    temp->left = temp->right = temp->p =  NULL;
    return temp;
}

stazione *insert_station(stazione *root,stazione *z){
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


void stampa_station(stazione *root,FILE *f){
    if(root!=NULL){
        fprintf(f,"%d ",root->dist);
        stampa_station(root->left,f);
        stampa_station(root->right,f);
    }
    else
        printf("- ");
}

stazione *search_station(stazione *root,int key){
    if(root == NULL || root->dist == key)
        return root;
    if(root->dist > key)
        return search_station(root->left,key);
    else
        return search_station(root->right,key);
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


stazione *transplant(stazione *root,stazione *u,stazione *v){
    if(u->p == NULL)
        root = v;
    else{
        if(u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
    }
    if(v != NULL)
        v->p = u->p;

    return root;
}

stazione *delete_station(stazione *root,stazione *z){
	//Possiamo avere 3 casi: se il nodo da eliminare a 1,2 o 0 figli
    stazione *y;
    if(z->left == NULL)
        root = transplant(root,z,z->right);
    else{
        if(z->right == NULL)
            root = transplant(root,z,z->left);
        else{
            y = min(z->right);
            if(y->p != z){
                root = transplant(root,y,y->right);
                y->right = z->right;
                y->right->p = y;
            }
            root = transplant(root,z,y);
            y->left = z->left;
            y->left->p = y;
        }
    }

    free(z->tab);
    free(z);
    return root;
}

lista *add_list(stazione *scroll){

        lista *curr = (lista *) malloc(sizeof(lista));
        curr->station = scroll;
        curr->pi = curr->prox = NULL;
        return curr;
}

lista *search_list(lista *t,int d){
    while(t != NULL && t->station->dist!= d)
        t = t->prox;
    return t;
}

void ripulisci_color(stazione *root){
    if(root != NULL){
        root->color = 'w';
        ripulisci_color(root->right);
        ripulisci_color(root->left);
    }
}


int plan(int start,int end,FILE *f,stazione *rad){
    if(start == end){
        fprintf(f,"%d",start);
        return 0;
    }

    stazione *scroll;                                                               //creo la lista di appoggio per non dover fare ogni volta succ(temp) da O(log n)
    lista *curr,*prec = NULL,*testa_lista = (lista *) malloc(sizeof(lista));
    scroll = min(rad);
    if(scroll == NULL){
        fprintf(f,"nessun percorso");
        return 0;
    }


    testa_lista->prox = NULL;
    testa_lista->pi = NULL;
    testa_lista->station = scroll;
    scroll = succ(scroll);
    prec = testa_lista;

    while(scroll != NULL){                                                         //inserisco elementi della lista, sarà ordinata
        curr = add_list(scroll);

        scroll = succ(scroll);
        prec->prox = curr;
        prec = prec->prox;

    }

    if(start < end){   //percorso normale

        lista *sx,*dx,*fine;
        lista *inizio = search_list(testa_lista,start);

        dx = fine = search_list(testa_lista,end);

        while(dx->station->dist != start){
            sx = inizio;

            while( (sx->station->tab->max + sx->station->dist) < dx->station->dist)
                sx = sx->prox;                                         //O(1)

            if(sx == dx){
                fprintf(f,"nessun percorso");
                    //libero la lista
                    lista *appoggio;
                    while(testa_lista != NULL){
                        appoggio = testa_lista;
                        testa_lista = testa_lista->prox;
                        appoggio->prox = NULL;
                        free(appoggio);
                    }                               //O(n)
                    return 0;
            }
            dx->pi = sx;
            dx = sx;
        }


        fprintf(f,"%d",inizio->station->dist);
        print_path_reverse(inizio,fine,f);


    }
    else{               //percorso inverso start > end
        stazione *upper_bound = NULL;
        lista *temp,*u,*small,*big;
        bool found = 0;

        big  = search_list(testa_lista,start);
        small = search_list(testa_lista,end);
        if(big->prox != NULL)
            upper_bound = big->prox->station;
        small->station->color = 'g';
        enqueue(small);
        while(head != NULL){
            u = temp = dequeue();

            while(temp != NULL && temp->station != upper_bound){
                    if(temp->station->color == 'w' && (temp->station->dist - temp->station->tab->max) <= u->station->dist){
                        temp->station->color = 'g';
                        temp->pi = u;
                        if(temp->station->dist == big->station->dist){
                            found = 1;
                            break;
                        }
                        enqueue(temp);
                    }
                temp = temp->prox;                      //O(1)
            }
            u->station->color = 'b';
            if(found == 1)
                break;
        }
        if(found == 1){
            print_path(small,big,f);
            fprintf(f,"%d",small->station->dist);
            while(head != NULL){
                dequeue();
            }
        }
        else
            fprintf(f,"nessun percorso");
    }//fine percorso inverso

    //libero la lista
    lista *appoggio;
    while(testa_lista != NULL){
        appoggio = testa_lista;
        testa_lista = testa_lista->prox;
        appoggio->prox = NULL;
        free(appoggio);
    }

    ripulisci_color(rad);
    return 0;
}

void libera_stazioni(stazione *root){
    if(root != NULL){
        libera_stazioni(root->left);
        libera_stazioni(root->right);
        free(root->tab);
        free(root);
    }
}

int main(){
	char buff[30];
	int i,dist,n,auton,batt,inizio,fine,posiz;
	FILE *fi,*fo;
    stazione *temp,*root;
    root = NULL;
    fi = stdin; // fi = fopen("input.txt","r");
	fo = fopen("output.txt","w");

	while( fscanf(fi,"%s",buff) != EOF){ //ERRORE QUI

        dist = 0;
        batt = 0;
        if(strcmp("aggiungi-stazione",buff)==0){
            if(fscanf(fi,"%d %d",&dist,&n)!= EOF);
            if(search_station(root,dist) == NULL){ //ovvero non è presente nell'albero
                temp = crea_node(dist);
                root = insert_station(root,temp);
                temp->tab->count = n;
                for(i=0;i<n;i++){
                    if(fscanf(fi," %d",&auton)!= EOF);
                    insert_hash(auton,temp->tab);
                }
                fprintf(fo,"aggiunta\n");
            }
            else                                   //ovvero è già presente
                fprintf(fo,"non aggiunta\n");
            continue;//Fine aggiungi-stazione
		}

		if(strcmp("demolisci-stazione",buff)==0){
			if(fscanf(fi,"%d",&dist)!= EOF);
            temp = search_station(root,dist);
            if(temp == NULL) //non presente
                fprintf(fo,"non demolita\n");
            else{
                root = delete_station(root,temp);
                fprintf(fo,"demolita\n");
            }
            continue;//Fine
        }

		if(strcmp("aggiungi-auto",buff)==0){
			if(fscanf(fi,"%d %d",&dist,&batt)!= EOF);
            temp = search_station(root,dist);
            if(temp == NULL)
                fprintf(fo,"non aggiunta\n");
            else{
                insert_hash(batt,temp->tab);
                fprintf(fo,"aggiunta\n");
            }
            continue;//Fine
        }
		if(strcmp("rottama-auto",buff)==0){
			if(fscanf(fi,"%d %d",&dist,&batt)!= EOF);
            temp = search_station(root,dist);
            if(temp == NULL)
                fprintf(fo,"non rottamata\n");
            else{
                posiz = search_hash(temp->tab,batt);
                if(posiz == -1)
                    fprintf(fo,"non rottamata\n");
                else{
                    delete_hash(posiz,temp->tab);
                    fprintf(fo,"rottamata\n");
                }
            }
            continue;//Fine
		}

		if(strcmp("pianifica-percorso",buff)==0){
			if(fscanf(fi,"%d %d",&inizio,&fine)!= EOF);
            plan(inizio,fine,fo,root);
            fprintf(fo,"\n");
            continue;//Fine
        }


    //Ancora dentro while feof
    }
    // stampa_station(root,fo);

    fclose(fi);
	fclose(fo);

    libera_stazioni(root);

	return 0;


}
