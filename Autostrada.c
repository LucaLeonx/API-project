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
    bool flag;
    Htable *tab;
    struct nodo *left,*right,*p;
}stazione;

// Funzioni tabelle hash
Htable *crea_tabella();
void new_table_max(Htable *map);
int insert_hash(int key,Htable *map);
int search_hash(Htable *map,int key);    //ritorna la posizione dell'elemento trovato, altrimeti ritorna -1
void stampa_hash(Htable *map,FILE *f);
void delete_hash(int pos,Htable *map);

// Funzioni BST
stazione *crea_node(int far);
stazione *succ(stazione *x);
stazione *prec(stazione *x);
stazione *insert_station(stazione *root,stazione *z);
stazione *search_station(stazione *root,int key);
stazione *max(stazione *root);
stazione *min(stazione *root);
stazione *delete_station(stazione *root,stazione *z);
void stampa_station(stazione *root,FILE *f);

void stampa_flag(stazione *root,FILE *f);
void ripulisci_flag(stazione *root);
int plan(int start,int end,FILE *f,stazione *rad);

int main(){
	char buff[30];
	int i,dist,n,auton,batt,inizio,fine,posiz;
	FILE *fi,*fo;
    stazione *root,*temp;
    root = (stazione *) malloc (sizeof(stazione));
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


	return 0;
}


Htable *crea_tabella(){
    Htable *t = (Htable *) malloc(sizeof(Htable));
    int i;
    t->count = 0;
    t->max = -1;
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
    }while( map->el[j] != -2 || i != cap);
    return -1;
}

void new_table_max(Htable *map){
    int i;
    map->max = -1;
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
    temp->flag = 0;
	temp->tab = crea_tabella();
    temp->left = temp->right = temp->p = NULL;
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

stazione *prec(stazione *x){
    stazione *y;
    if(x->left != NULL)
        return max(x->left);
    y = x->p;
    while(y!=NULL && x == y->left){
        x = y;
        y = y->p;
    }
    return y;
}

stazione *delete_station(stazione *root,stazione *z){
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
    if(y!=z){
        z->dist = y->dist; 													// e eventualmete altri dati (fatto?)
		z->tab = y->tab;
    }
    free(y->tab);
    free(y);
    return root;
}

void ripulisci_flag(stazione *root){
    if(root != NULL){
        root->flag = 0;
        ripulisci_flag(root->left);
        ripulisci_flag(root->right);
    }
}

void stampa_flag(stazione *root,FILE *f){
    if(root != NULL){
        stampa_flag(root->left,f);
        if(root->flag == 1)
            fprintf(f," %d",root->dist);
        stampa_flag(root->right,f);
    }
}

void stampa_flag_inv(stazione *root,FILE *f){
    if(root != NULL){
        stampa_flag_inv(root->right,f);
        if(root->flag == 1)
            fprintf(f,"%d ",root->dist);
        stampa_flag_inv(root->left,f);
    }
}


int plan(int start,int end,FILE *f,stazione *rad){
    if(start == end){
        fprintf(f,"%d",start);
        return 0;
    }


    if(start < end){   //percorso normale
        stazione *sx,*dx;
        stazione *inizio = search_station(rad,start);

        dx = search_station(rad,end);

        while(dx->dist != start){
            sx = inizio;

            while( (sx->tab->max + sx->dist) < dx->dist)
                sx = succ(sx);                                         //O(log n)

            if(sx == dx){
                fprintf(f,"nessun percorso");
                    ripulisci_flag(rad);                               //O(n)
                    return 0;
            }
            dx->flag = 1;
            dx = sx;
        }

        fprintf(f,"%d",inizio->dist);
        stampa_flag(rad,f);
        ripulisci_flag(rad);
    }
    else{               //percorso inverso SBAGLIATO
        stazione *sx,*dx;
        stazione *inizio = search_station(rad,end);

        dx = search_station(rad,start);

        while(dx->dist != end){
            sx = inizio;

            while( sx->dist < (dx->dist - dx->tab->max) )
                sx = succ(sx);                                         //O(log n)

            if(sx == dx){
                fprintf(f,"nessun percorso");
                    ripulisci_flag(rad);                               //O(n)
                    return 0;
            }
            dx->flag = 1;
            dx = sx;
        }

        stampa_flag_inv(rad,f);
        fprintf(f,"%d",inizio->dist);
        ripulisci_flag(rad);
    }

    return 0;
}
