#include <iostream>
#include <cstdlib>
#include <string>
#include "database.h"
using namespace std;

// 동적할당으로 만든 배열의 주소 반환
void addArray(Array* arr){
    Type type;
    string typeStr;
    int size;
    void* value;
    
    // type 입력
    while(true){
        cout<<"type (int, double, string, array): ";
        cin>>typeStr;
        if(typeStr=="int") type=INT;
        else if(typeStr=="double") type=DOUBLE;
        else if(typeStr=="string") type=STRING;
        else if(typeStr=="array") type=ARRAY;
        else{
            cout<<"invalid type!\n";
            continue;
        }
        break;
    }
    arr->type=type;

    // size 입력
    cout<<"size: ";
    cin>>size;
    arr->size=size;
    
    switch(type){
        case INT:
            value=new int[size];
            for(int i=0;i<size;i++){
                cout<<"item["<<i<<"]: ";
                cin>>((int*)value)[i];
            }
            break;
        case DOUBLE:
            value=new double[size];
            for(int i=0;i<size;i++){
                cout<<"item["<<i<<"]: ";
                cin>>((double*)value)[i];
            }
            break;
        case STRING:
            value=new string[size];
            for(int i=0;i<size;i++){
                cout<<"item["<<i<<"]: ";
                cin>>((string*)value)[i];
            }
            break;
        case ARRAY:
            value=new Array[size];
            for(int i=0;i<size;i++){
                cout<<"item["<<i<<"]: ";
                addArray(((Array*)value)+i);
            }
            break;
    }
    arr->items=value;
}

void printArray(Array* array){
    int size=array->size;
    Type type=array->type;

    int* intArr;
    double* doubleArr;
    string* stringArr;
    Array* arrArr;

    cout<<"[";
    switch(type){
        case INT:
            intArr=(int*)array->items;
            for(int i=0;i<size;i++){
                cout<<intArr[i];
                if(i!=size-1) cout<<", ";
            }
            break;
        case DOUBLE:
            doubleArr=(double*)array->items;
            for(int i=0;i<size;i++){
                cout<<doubleArr[i];
                if(i!=size-1) cout<<", ";
            }
            break;
        case STRING:
            stringArr=(string*)array->items;
            for(int i=0;i<size;i++){
                cout<<"\""<<stringArr[i]<<"\"";
                if(i!=size-1) cout<<", ";
            }
            break;
        case ARRAY:
            arrArr=(Array*)array->items;
            for(int i=0;i<size;i++){
                printArray(arrArr+i);
                if(i!=size-1) cout<<", ";
            }
            break;
    }

    cout<<"]";
}

void printEntry(Entry* entry){
    switch(entry->type){
        case INT:
            cout<<*((int*)entry->value);
            break;
        case DOUBLE:
            cout<<*((double*)entry->value);
            break;
        case STRING:
            cout<<"\""<<*((string*)entry->value)<<"\"";
            break;
        case ARRAY:
            printArray((Array*)entry->value);
            break;
    }
}

void commandList(Database& db){
    Node* cur=db.tail;
    while(cur!=nullptr){
        cout<<cur->entry->key<<": ";
        printEntry(cur->entry);
        cout<<"\n";
        cur=cur->prev;
    }
}

void commandAdd(Database& db){
    // key 입력
    string key;
    cout<<"key: ";
    cin>>key;
    
    // type 입력
    Type type;
    string typeStr;
    while(true){
        cout<<"type (int, double, string, array): ";
        cin>>typeStr;
        if(typeStr=="int") type=INT;
        else if(typeStr=="double") type=DOUBLE;
        else if(typeStr=="string") type=STRING;
        else if(typeStr=="array") type=ARRAY;
        else{
            cout<<"invalid type!\n";
            continue;
        }
        break;
    }
    
    // value 입력
    void* value;
    cout<<"value: ";
    switch(type){
        case INT:
            value=new int;
            cin>>*((int*)value);
            break;
        case DOUBLE:
            value=new double;
            cin>>*((double*)value);
            break;
        case STRING:
            value=new string;
            getline(cin, *((string*)value)); // type 입력하고 엔터친 거 회수
            getline(cin, *((string*)value));  // 한 줄 입력
            break;
        case ARRAY:
            value=new Array;
            addArray((Array*)value);
            break;
    }
    Entry* entry=create(type, key, value);
    add(db, entry);
}

void commandGet(Database& db){
    string key;

    // key 입력
    cout<<"key: ";
    cin>>key;

    // entry 검색
    Entry* entry=get(db, key);
    if(entry==nullptr) cout<<"Not Found : 해당 key가 존재하지 않습니다.\n";
    else {
        cout<<entry->key<<": ";
        printEntry(entry);
        cout<<"\n";
    }
}

void commandDel(Database& db){
    string key;
    cout<<"key: ";
    cin>>key;
    remove(db, key);
}

void shell(){
    // DB 생성
    Database db;
    init(db);

    string command;
    while(true){
        // 프롬프트 출력과 명령어 입력
        cout<<"command (list, add, get, del, exit): ";
        cin>>command;

        // command 해석하고 적절한 함수 호출
        if(command=="list") commandList(db);
        else if(command=="add") commandAdd(db);
        else if(command=="get") commandGet(db);
        else if(command=="del") commandDel(db);
        else if(command=="exit") break;
        else cout<<"invalid command!\n";
        cout<<"\n";
    }

    // DB 해제 (메모리 할당 해제)
    destroy(db);
}