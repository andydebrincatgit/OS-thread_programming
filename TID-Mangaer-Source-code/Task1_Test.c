#include "tid_map.h"


int main(){

    printf("This is a test\n");
    if(allocate_map() == -1){
        printf( "Could not allocate a map\n");
        return -1;
    };

    printf("\n\n");
    for(int i = 1; i <= 4702; i++){
        int tid = allocate_tid();
        if(tid==-1){
            printf("Could not allocate for iteration number %d\n", i);
        }
    }

    printf("\n\n");

    for(int i = 299; i<= 5001; i++){
        release_tid(i);

    }

    printf("\n\n");

    for(int i = 1; i <= 4701; i++){
        int tid = allocate_tid();
        if(tid==-1){
            printf("Could not allocate for iteration number %d\n", i);

        }
    }
    printf("Test successful\n");

    free(tidMap_ptr->tidList);
    free(tidMap_ptr);

}
