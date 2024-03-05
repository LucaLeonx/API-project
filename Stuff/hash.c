#include <stdlib.h>
#include <stdio.h>

#define cap 512

typedef struct tabella{
    int el[512];
    int count;
    int max;
}Htable;

Htable *crea_tabella();
void new_table_max(Htable *map);
int insert(int key,Htable *map);
int search(Htable *map,int key);    //ritorna la posizione dell'elemento trovato, altrimeti ritorna -1
void stampa(Htable *map);
void delete(int key,Htable *map);

int main(){

    Htable *table = crea_tabella();
    insert(1,table);
    insert(5,table);
    insert(6,table);
    insert(3,table);
    insert(513,table);
    insert(7,table);
    insert(0,table);
    insert(514,table);

    stampa(table);

    printf("\n dopo le delete: \n");

    delete(5,table);
    delete(514,table);

    stampa(table);
    printf("\n");

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

int insert(int key,Htable *map){
    int x,j=0,i;
    map->count++;
    for(i=0;i<cap;i++){
        j = (key+i) % 512;
        if(map->el[j] == -2 || map->el[j] == -1){          // -1 equivale a DELETE; -2 equivale a NULL
            map->el[j] = key;
            if(key>map->max)
                map->max = key;
            return 1;
        }
    }
    map->count--;
    return 0;
}

void stampa(Htable *map){
    int i;
    for(i=0;i<20;i++){
        printf(" %d",map->el[i]);
    }
    printf("\n %d \n",map->max);
}

int search(Htable *map,int key){
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

void delete(int key,Htable *map){
    int pos;
    pos = search(map,key);
    if(pos!= -1)
        map->el[pos] = -1;
    else
        printf("elemento da eliminare non esistente \n");
    map->count--;
    if(key == map->max)
        new_table_max(map);
}
