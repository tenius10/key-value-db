#include <cstdlib>
#include <string>
#include "database.h"

// 엔트리를 생성한다.
Entry *create(Type type, std::string key, void *value){
    // 동적으로 Entry 할당 후 반환
    Entry* entry=new Entry;
    entry->type=type;
    entry->value=value;
    entry->key=key;
    return entry;
}

// 데이터베이스를 초기화한다.
void init(Database &database){
    database.head=nullptr;
    database.tail=nullptr;
}

// 데이터베이스에 엔트리를 추가한다.
void add(Database &database, Entry *entry){
    // entry를 node로 감싼 후, 연결 리스트의 끝에 연결
    // * 주의 : 이미 있는 key라면 type과 value만 바꾸면 됨
    Entry* searchedEntry=get(database, entry->key);
    
    if(searchedEntry!=nullptr){
        searchedEntry->value=entry->value;
        searchedEntry->type=entry->type;
        // 내용을 copy했으니 entry는 할당 해제
        delete entry;
    }
    else{
        Node* node=new Node;
        node->entry=entry;

        if(database.tail==nullptr){
            node->prev=nullptr;
            node->next=nullptr;

            database.head=node;
            database.tail=node;
        }
        else{
            node->prev=database.tail;
            node->next=nullptr;
            
            database.tail->next=node;
            database.tail=node;
        }
    }
}

// 데이터베이스에서 키에 해당하는 엔트리를 찾는다.
Entry *get(Database &database, std::string &key){
    // head에서부터 순차적으로 link를 따라가며 탐색
    Node* cur=database.head;
    while(cur!=nullptr){
        if(cur->entry->key==key){
            return cur->entry;
        }
        cur=cur->next;
    }
    return nullptr;  // key가 없으면 null 반환
}

void freeNestedArray(Array* array){
    // items는 new Type[size] 형태로 할당했으니 delete[]로 해제함에 주의
    switch(array->type){
        case INT:
            delete[] (int*)array->items;
            break;
        case DOUBLE:
            delete[] (double*)array->items;
            break;
        case STRING:
            delete[] (std::string*)array->items;
            break;
        case ARRAY:
            int size=array->size;
            Array* items=(Array*)array->items;
            for(int i=0;i<size;i++){
                freeNestedArray(items+i);
            }
            delete[] (Array*)array->items;
            break;
    }
}

// Node, Entry, Entry->value 일괄 삭제
void freeNode(Node* node){
    switch(node->entry->type){
        case INT:
            delete (int*)node->entry->value;
            break;
        case DOUBLE:
            delete (double*)node->entry->value;
            break;
        case STRING:
            delete (std::string*)node->entry->value;
            break;
        case ARRAY:
            freeNestedArray((Array*)node->entry->value);
            delete (Array*)node->entry->value;
            break;
    }

    // entry, node 할당 해제
    delete node->entry;
    delete node;
}

// 데이터베이스에서 키에 해당하는 엔트리를 제거한다.
void remove(Database &database, std::string &key){
    // 키 발견하면 노드 삭제하고 앞/뒤 노드 적당히 연결
    Node* cur=database.head;
    while(cur!=nullptr){
        if(cur->entry->key==key){
            // 만약 내가 head라면, head를 내 뒤의 노드로 변경해야 함.
            if(database.head==cur) database.head=cur->next;
            // 만약 내가 tail이라면, tail을 내 앞의 노드로 변경해야 함.
            if(database.tail==cur) database.tail=cur->prev;

            // 할당 해제 전에 앞/뒤 노드 연결
            Node* next=cur->next;
            if(next!=nullptr) next->prev=cur->prev;
            Node* prev=cur->prev;
            if(prev!=nullptr) prev->next=cur->next;

            freeNode(cur);
            break;
        }
        else cur=cur->next;
    }
}

// 데이터베이스를 해제한다.
void destroy(Database &database){
    // 연결 리스트를 순회하며 하나씩 할당 해제
    Node* cur=database.tail;
    Node* prev;
    while(cur!=nullptr){
        prev=cur->prev;
        freeNode(cur);
        cur=prev;
    }
    database.head=nullptr;
    database.tail=nullptr;
}