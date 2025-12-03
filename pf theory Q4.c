#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Book{
    int ID;
    int popularity;
    int last_access;  
};
void ADD_book(int x, int y, struct Book* shelf, int capacity, int *current_time);
int ACCESS_book(int x, struct Book* shelf, int capacity, int *current_time);
int main(){
    int capacity=0;
    int Q=0;
    printf("Enter capacity and number of operations:");
    scanf("%d %d",&capacity,&Q);
    struct Book* shelf = (struct Book*)malloc(capacity * sizeof(struct Book));
    int i=0;
    for(i=0;i<capacity;i++){
        shelf[i].ID = -1; 
        shelf[i].popularity = 0;
        shelf[i].last_access = -1;
    }
    int current_time = 1;
    char operation[20];
    int x=0;
    int y=0;
    for(i=0;i<Q;i++){
        scanf("%s",operation);
        if(strcmp(operation,"ADD")==0){
            scanf("%d %d",&x,&y);
            ADD_book(x,y,shelf,capacity,&current_time);
        }
        else if(strcmp(operation,"ACCESS")==0){
            scanf("%d",&x);
            int result = ACCESS_book(x,shelf,capacity,&current_time);
            printf("%d\n",result);
        }
    }
    free(shelf);
    return 0;
}
void ADD_book(int x, int y, struct Book* shelf, int capacity, int *current_time){
    int i=0;
    for(i=0;i<capacity;i++){
        if(shelf[i].ID == x){
            shelf[i].popularity = y;
            shelf[i].last_access = *current_time;
            (*current_time)++;
            return;
        }
    }
    for(i=0;i<capacity;i++){
        if(shelf[i].ID == -1){
            shelf[i].ID = x;
            shelf[i].popularity = y;
            shelf[i].last_access = *current_time;
            (*current_time)++;
            return;
        }
    }
    int oldest_index = 0;
    int oldest_time = shelf[0].last_access;
    for(i=1;i<capacity;i++){
        if(shelf[i].last_access < oldest_time){
            oldest_time = shelf[i].last_access;
            oldest_index = i;
        }
    }
    shelf[oldest_index].ID = x;
    shelf[oldest_index].popularity = y;
    shelf[oldest_index].last_access = *current_time;
    (*current_time)++;
}
int ACCESS_book(int x, struct Book* shelf, int capacity, int *current_time){
    int i=0;

    for(i=0;i<capacity;i++){
        if(shelf[i].ID == x){
            shelf[i].last_access = *current_time;
            (*current_time)++;
            return shelf[i].popularity;
        }
    }
    return -1;
}

