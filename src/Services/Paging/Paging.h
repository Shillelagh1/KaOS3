#define PAGE_DIR_LEN 1024
#define PAGE_WIDTH 4194304

#define PDE_PRESENT     1
#define PDE_RW          2
#define PDE_US          4
#define PDE_PWT         8
#define PDE_PCD         16
#define PCE_ACCESSED    32
#define PCE_DIRTY       64

struct page_directory_entry {
    unsigned int attr   : 7;
    unsigned int one    : 1;
    unsigned int global : 1;
    unsigned int avl0   : 3;
    unsigned int zero0  : 1;
    unsigned int addr_high   : 8;
    unsigned int zero1  : 1;
    unsigned int addr_low  : 10;
}   __attribute__((packed));

#define CREATE_PAGEDIR(__name) struct page_directory_entry page_dir_##__name[1024] __attribute__((aligned(4096)))

void PG_ZeroizePagedir(struct page_directory_entry* directory);
void PG_ZeroizePagedirEntry(struct page_directory_entry* directoryEntry);
void PG_ConfigurePagedirEntry(struct page_directory_entry* entry, int attr, int global, int pageNumber);
void PG_EnablePaging(struct page_directory_entry* directory);