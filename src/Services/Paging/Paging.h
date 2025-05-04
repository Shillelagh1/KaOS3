#define PDE_PRESENT     1
#define PDE_RW          2
#define PDE_US          4
#define PDE_PWT         8
#define PDE_PCD         16
#define PCE_ACCESSED    32

struct page_directory_entry {
    unsigned int attr   : 6;
    unsigned int avl0   : 1;
    unsigned int zero   : 1;
    unsigned int avl1   : 4;
    unsigned int addr   : 20;
}   __attribute__((packed));

#define PTE_PRESENT     1
#define PTE_RW          2
#define PTE_US          4
#define PTE_PWT         8
#define PTE_PCD         16
#define PTE_ACCESSED    32
#define PTE_DIRTY       64
#define PTE_PAT         128
#define PTE_GLOBAL      256   

struct page_table_entry {
    unsigned int attr : 9;
    unsigned int avl  : 3;
    unsigned int addr : 20;
} __attribute__((packed));

#define CREATE_PAGEDIR(__name) struct page_directory_entry page_dir___name[1024] __attribute__((aligned(4096)))
#define CREATE_PAGETABLE(__name, __n) struct page_table_entry page_table___name___n[1024] __attribute__((aligned(4096)))
#define CREATE_FULL_PAGETABLE(__name) struct page_table_entry page_table___name[1024 * 1024] __attribute__((aligned(4096)))

void PG_ZeroizePagedir(struct page_directory_entry* directory);
void PG_ZeroizePagetable(struct page_table_entry* table);