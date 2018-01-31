#include<cstring>
#include<iostream>
#include<list>
#include "symtab.hpp"

SYMTAB_NODE *currInstNode = NULL;

SYMTAB_NODE *SymbolTable::get_new_node(char* symbol, int linenumber, int size, bool is_array, int array_size,
                                        int type)
{
    SYMTAB_NODE *newnode = NULL;
    newnode = new SYMTAB_NODE[sizeof(SYMTAB_NODE)];
    if(newnode != NULL){
        newnode->symbol = new char[strlen(symbol)];
        newnode->symbol = symbol;
        newnode->linenumber = linenumber;
        newnode->size = size;
        newnode->is_array = is_array;
        newnode->array_size = array_size;
        newnode->type = type;
        newnode->addr_list = NULL;
        newnode->next = NULL;
    }
    return newnode;
}


void SymbolTable::add_node(SYMTAB_NODE **head, char* symbol, int linenumber, int size, bool is_array, 
                            int array_size, int type)
{
    SYMTAB_NODE *current = *head;
    if(current==NULL){
        *head = get_new_node(symbol, linenumber, size, is_array, array_size, type);
        currInstNode = *head;
    }else{
    while(current->next != NULL){
        current=current->next;
    }
    current->next = get_new_node(symbol, linenumber, size, is_array, array_size, type);
    currInstNode = current->next;
    }
}


int SymbolTable::get_hash_code(int key)
{
    return key%TABLE_SIZE;
}


void SymbolTable::insert(char* symbol, int linenumber, int size, bool is_array, int array_size, int type)
{
    unsigned int hashCode = get_hash_code(strlen(symbol));
    add_node(&(head[hashCode]), symbol, linenumber, size, is_array, array_size, type);
}


bool SymbolTable::search(char* symbol)
{
    unsigned int hashCode = get_hash_code(strlen(symbol));
    SYMTAB_NODE *bucknode = head[hashCode];
    while(bucknode !=NULL){
        if((strcmp(bucknode->symbol, symbol)) == 0){
            return true;
        }
        bucknode = bucknode->next;
    }
    return false;
}



SYMTAB_NODE *SymbolTable::search_node(char* symbol)
{
    unsigned int hashCode = get_hash_code(strlen(symbol));
    SYMTAB_NODE *bucknode = head[hashCode];
    while(bucknode !=NULL){
        if((strcmp(bucknode->symbol, symbol)) == 0){
            return bucknode;
        }
        bucknode = bucknode->next;
    }
    return NULL;
}

SYMTAB_NODE *SymbolTable::search_node_2(char* symbol)
{
    unsigned int hashCode = get_hash_code(strlen(symbol));
    SYMTAB_NODE *bucknode = head[hashCode];
    while(bucknode !=NULL){
        if((bucknode->type == VAR_DECL || bucknode->type == RES_DECL) && (strcmp(bucknode->symbol, symbol)) == 0 ){
          return bucknode;
        }
        bucknode = bucknode->next;
    }
    return NULL;
}


VAL_LIST_NODE *SymbolTable::get_new_value_node(char* value, int type)
{
    VAL_LIST_NODE *newvalnode = NULL;
    newvalnode = new VAL_LIST_NODE[sizeof(VAL_LIST_NODE)];
    if(newvalnode!=NULL){
        newvalnode->value = new char[strlen(value)];
        newvalnode->value = value;
        newvalnode->type = type;
        newvalnode->next = NULL;
    }
    return newvalnode;
}



void SymbolTable::insert_value(VAL_LIST_NODE **valuehead, char* value, int type)
{
    VAL_LIST_NODE *current = *valuehead;
    if(current==NULL){
        *valuehead = get_new_value_node(value, type);
    }else{
        while(current->next!=NULL){
            current=current->next;
        }
        current->next = get_new_value_node(value, type);
    }
}


int SymbolTable::getLength_values_list(SYMTAB_NODE *head)
{
    int len = 0;
    VAL_LIST_NODE *current = head->values_list;
    while(current != NULL){
        if(current->value != NULL){
            len += strlen(current->value);
        }
        current = current->next;
    }
    return len;
}



void SymbolTable::display()
{
    for(int i=0;i<TABLE_SIZE;i++){
        SYMTAB_NODE *current = head[i];
        while(current != NULL){
            std::cout<<"symbol = "<<current->symbol<<" linenumber = "<<current->linenumber
            <<", size = "<<current->size<<", is_array = "<<current->is_array<<
            ", array_size = "<<current->array_size<<", type = "<<current->type<<std::endl;

            while(current->values_list != NULL){
                std::cout<<"value = "<<(current->values_list)->value<<
                "  type = "<<(current->values_list)->type<<std::endl;
                current->values_list = (current->values_list)->next;
            }
            
            if(current->addr_list != NULL){
                std::list<int>::iterator p = current->addr_list->begin();
                while(p != current->addr_list->end()){
                    std::cout<<"address = "<<std::hex<<*p<<std::endl;
                    p++;
                }
            }
            current=current->next;
        }
    }
}