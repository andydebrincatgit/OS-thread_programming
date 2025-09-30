#ifndef MYHEADER_H
#define MYHEADER_H


#define MINTID 300
#define MAXTID 5000
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


//The data structure chosen is an array of structures, where the structure is a simple tid with an integer value and a boolean flag to represent its availaibility.

typedef struct{
    int tid;
    bool in_use;
}tid;

//Another structure was created so that in addition to the array itself, there will also be a pointer that indicates where a free tid is, in order to speed up the process of finding a free tid.

typedef struct{
    tid *tidList;
    int free_ptr;
}tidMap;

//The tid_map is set as a global variable to mimic a shared resource.

tidMap * tidMap_ptr = NULL;


//part b : the allocate map function simply allocates memory on the heap for the tidMap structure, and initializes the free pointer to 0, to indicate that there is a free tid at the index 0.

int allocate_map(){

    tidMap_ptr = (tidMap *)malloc(sizeof(tidMap));

    if(tidMap_ptr == NULL){
        return -1;
    }

    tidMap_ptr->free_ptr = 0;
    tidMap_ptr->tidList = (tid *)malloc((MAXTID - MINTID + 1)*sizeof(tid));

    if(tidMap_ptr->tidList == NULL){
        return -1;
    }

    for(int i = 0; i <= MAXTID - MINTID; i++){
        tidMap_ptr->tidList[i].tid = MINTID + i;
        tidMap_ptr->tidList[i].in_use = false;
    }
    return 1;
}


//part c : 

int allocate_tid(){

  //the allocate tid function first checks if the free pointer is -1, which indicates that there is no free tid. 
    int ret = 0;
    if(tidMap_ptr->free_ptr == -1){
        return -1;
    }
    else{
      //Then it sets the return value to the value that the free pointer indexes in the array if it is not -1.
      //The in_use flag is set to true.
        ret = tidMap_ptr->tidList[tidMap_ptr->free_ptr].tid;
        tidMap_ptr->tidList[tidMap_ptr->free_ptr].in_use = true;
    }

  //Then  and searches for the next free tid, in a circlar fashion starting from the free pointer.
    int ct_1 = tidMap_ptr->free_ptr;
    int ct_2 = 0;
    bool found = false;

    do{
        if(ct_1==MAXTID-MINTID){
            ct_1 = 0;
        }
        else{
            ct_1++;
        }
        ct_2++;

        if(tidMap_ptr->tidList[ct_1].in_use == false){
            tidMap_ptr->free_ptr = ct_1;
            found = true;
        }

    }
    while((ct_2<MAXTID-MINTID) && !found);

  //If no free tid is found, the free pointer is set to -1 for the next call to allocate tid.
    if(!found){
        tidMap_ptr->free_ptr = -1;
    }

    return ret;
}

//part d : This simply releases a given tid by setting the in_use flag to false, and the free pointer to the index of the tid.
void release_tid(int tid){
    if(tid>MAXTID || tid<MINTID){
        printf("%d is an invalid tid\n", tid);
        return;
    } 
    tidMap_ptr->tidList[tid-MINTID].in_use = false;
    tidMap_ptr->free_ptr = tid-MINTID;
}


#endif
