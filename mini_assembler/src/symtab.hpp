#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include<iostream>

#define TABLE_SIZE 10

#define DATA_SECTION 1001
#define BSS_SECTION 1002
#define TEXT_SECTION 1003


#define VAR_DECL 100
#define RES_DECL 111
#define LAB_DECL 120


#define NUM_TYPE 200
#define STR_TYPE 211


typedef struct __val_list_node{
    char* value;
    int type;
    struct __val_list_node *next;
}VAL_LIST_NODE;


//structure for symbol table
typedef struct __symtabnode{
    char* symbol;
    int linenumber;
    int size;
    bool is_array;
    int array_size;
    int type;
    std::list<int> *addr_list;
    VAL_LIST_NODE *values_list;
    struct __symtabnode *next;
}SYMTAB_NODE;


class SymbolTable{
    private: 
        SYMTAB_NODE *get_new_node(char*, int, int, bool, int, int);
        void add_node(SYMTAB_NODE **, char*, int, int, bool, int, int);
        int get_hash_code(int);
        VAL_LIST_NODE *get_new_value_node(char*, int);
    public:
        SYMTAB_NODE *head[TABLE_SIZE];
        void insert(char*, int, int, bool, int, int);
        bool search(char*);
        SYMTAB_NODE *search_node(char*);
        SYMTAB_NODE *search_node_2(char*);
        void insert_value(VAL_LIST_NODE **, char*, int);
        int getLength_values_list(SYMTAB_NODE *);
        void display();
};

#endif

