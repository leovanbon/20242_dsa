#include <stdio.h>
#include <stdlib.h>
typedef struct MemoryBlock{
    void* start_addr;
    size_t size;
    struct MemoryBlock* next;
} MemoryBlock;

MemoryBlock* free_list = NULL;
MemoryBlock* nextfit_sp = NULL;

void init_mem(void* base_addr, size_t total_size){
    free_list = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    free_list->start_addr = base_addr;
    free_list->size = total_size;
    free_list->next = NULL;
}

void print_free_list(){
    MemoryBlock* cur = free_list;
    printf("FREE MEM BLOCKS:\n");
    while (cur != NULL){
        printf("    Address: %p, Size: %zu\n", cur->start_addr, cur->size);
        cur=cur->next;
    }
}


// cap phat don gian
void* firstfit_malloc(size_t WANT){
    MemoryBlock* cur = free_list;
    MemoryBlock* prev= NULL;

    while(cur != NULL){
        if(cur->size >= WANT){
            void* alloc = cur->start_addr;

            if (cur->size == WANT){
                if(prev == NULL) free_list = cur->next;
                else prev->next=cur->next;
                free(cur);
            }else{
                cur->start_addr = (char*)cur->start_addr + WANT;
                cur->size -= WANT;
            }
            return alloc;
        }

        prev = cur;
        cur = cur->next;
    }

    return NULL;
}


void* bestfit_malloc(size_t WANT){
    MemoryBlock* cur = free_list;
    MemoryBlock* prev = NULL;
    MemoryBlock* best_cur = NULL;
    MemoryBlock* best_prev = NULL;

    while (cur != NULL){
        if(cur->size >= WANT) {
            if (best_cur == NULL || cur->size < best_cur->size){
                best_cur = cur;
                best_prev = prev;
            }
        }

        prev=cur;
        cur=cur->next;
    }

    if(best_cur==NULL) return NULL;

    void* alloc = best_cur->start_addr;

    if (best_cur->size == WANT){
        if(best_prev == NULL) free_list = best_cur->next;
        else best_prev->next=best_cur->next;
        free(best_cur);
    }else{
        best_cur->start_addr = (char*)best_cur->start_addr + WANT;
        best_cur->size -= WANT;
    }
    return alloc;
}

void* worstfit_malloc(size_t WANT){
    MemoryBlock* cur = free_list;
    MemoryBlock* prev = NULL;
    MemoryBlock* worst_cur = NULL;
    MemoryBlock* worst_prev = NULL;

    while (cur != NULL){
        if(cur->size >= WANT) {
            if (worst_cur == NULL || cur->size > worst_cur->size){
                worst_cur = cur;
                worst_prev = prev;
            }
        }

        prev=cur;
        cur=cur->next;
    }

    if(worst_cur==NULL) return NULL;

    void* alloc = worst_cur->start_addr;

    if (worst_cur->size == WANT){
        if(worst_prev == NULL) free_list = worst_cur->next;
        else worst_prev->next=worst_cur->next;
        free(worst_cur);
    }else{
        worst_cur->start_addr = (char*)worst_cur->start_addr + WANT;
        worst_cur->size -= WANT;
    }
    return alloc;
}

void* nextfit_malloc(size_t WANT){
    if(free_list==NULL) return NULL;
    if(nextfit_sp=NULL) nextfit_sp=free_list;

    MemoryBlock* cur = nextfit_sp;
    MemoryBlock* prev = NULL;
    MemoryBlock* st = cur;

    while(1){
        if(cur->size >= WANT){
            void* alloc = cur->start_addr;

            if(cur->size == WANT){
                if(cur == free_list) free_list=cur->next;
                else prev->next=cur->next;

                nextfit_sp =   cur->next != NULL ? cur->next : free_list;

                free(cur);
            }else{
                cur->start_addr = (char*)cur->start_addr + WANT;
                cur->size -= WANT;
                nextfit_sp = cur;
            }

            return alloc;
        }

        prev=cur;
        cur = cur->next ? cur->next : free_list;

        if(cur=st) break;
    }
    return NULL;
}

size_t powerup(size_t WANT) {
    if (WANT == 0) return 1;
    size_t power = 1;
    while (power < WANT) power *= 2;
    return power;
}

void* buddysystem_malloc(size_t WANT){
    
}

void free_mem(void* addr, size_t WANT){

}



int main(){
    void* memory_pool = malloc(2048);
    init_mem(memory_pool, 2048);

    void* ptr1 = firstfit_malloc(256);
    printf("Allocated 256 bytes at %p\n", ptr1);
    print_free_list();

    return 0;
}