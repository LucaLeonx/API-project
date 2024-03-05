#include <stdio.h>
#include <stdlib.h>

typedef struct el_nodo{
    int d;
    struct el_nodo *next,*prec; //next va verso destra -> --- prec va verso sinistra <-
}el_queue;                      //la testa ha prec NULL e la coda ha next NULL

el_queue *head = NULL, *tail = NULL;

el_queue *enqueue(int staz){ //aggiungo l'elemento in coda
    el_queue *new_tail =(el_queue *) malloc(sizeof(el_queue));
    new_tail->d = staz;
    new_tail->next = new_tail->prec = NULL;
    if(tail == NULL)
        head = tail =  new_tail;
    else{
        new_tail->prec = tail;
        tail->next = new_tail;
        tail = new_tail;

    }
    return tail;
}

void dequeue(){ // tolgo elemento in testa
    if(head != NULL){
        el_queue *temp;
        temp = head;
        if(head->next != NULL){
            head = head->next;
            head->prec = NULL;
        }
        else
            head = NULL;
        free(temp);
    }
}

void clean_queue(){
    while(head != NULL){
         dequeue();
    }

}

void display()
{
    el_queue *temp = head;
    printf("\n");
    while (temp != NULL)
    {
        printf("%d\t", temp->d);
        temp = temp->next;
    }
}


int main(){

    int n, ch;
    do
    {
        printf("\n\nQueue Menu\n1. Add \n2. Remove\n3. Display\n0. Exit");
        printf("\nEnter Choice 0-3? : ");
        scanf("%d", &ch);
        switch (ch)
        {
            case 1:
                printf("\nEnter number ");
                scanf("%d", &n);
                enqueue(n);
                break;
            case 2:
                dequeue();
                break;
            case 3:
                display();
                break;
        }
    }while (ch != 0);

    clean_queue();

    return 0;
}
