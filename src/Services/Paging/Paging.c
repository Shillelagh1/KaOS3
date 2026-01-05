#include <stdint.h>
#include "Paging.h"

void PG_ZeroizePagedirEntry(struct page_directory_entry* entry){
    entry->attr = 0;
    entry->one = 0;
    entry->global = 0;
    entry->avl0 = 0;
    entry->zero0 = 0;
    entry->addr_low = 0;
    entry->zero1 = 0;
    entry->addr_high = 0;
}

void PG_ZeroizePagedir(struct page_directory_entry* directory){
    for(int i = 0; i < PAGE_DIR_LEN; i++){
        PG_ZeroizePagedirEntry(&directory[i]);
    }
}

void PG_ConfigurePagedirEntry(struct page_directory_entry* entry, int attr, int global, int pageNumber){
    entry->attr = attr;
    entry->one = 1;
    entry->global = global;
    entry->avl0 = 0;
    entry->zero0 = 0;
    
    entry->addr_low = (pageNumber) & 1023;
    entry->addr_high = (pageNumber >> 10) & 255;
}

extern void _pg_enable_paging(struct page_directory_entry*);

void PG_EnablePaging(struct page_directory_entry* directory){
    _pg_enable_paging(directory);
}