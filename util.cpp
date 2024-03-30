#include "database.h"
#include <iostream>
#include <string>
#include <stdlib.h>

Type selectType(std::string type){
  Type t;
  if(type == "int"){
    t = INT;
  }
  else if(type == "double"){
    t = DOUBLE;
  }
  else if(type == "string"){
    t = STRING;
  }
  else if(type == "array"){
    t = ARRAY;
  }
  return t;
}

Array *createArr(){
  std::string type;
  Type t;
  int size;
  std::cout << "type (int, double, string, array): ";
  std::cin >> type;
  t = selectType(type);
  std::cin >> size;
  void *iptr;
  Array *newArray = new Array;
  newArray -> type = t;
  newArray -> size = size;
  switch(t){
    case INT:
      int *items = new int[size];
      for(int i = 0; i < size; i++){
        std::cout << "item[" << i << "]";
        std::cin >> *(items + i);
      }
      iptr = items;
      break;
    case DOUBLE:
      double *items = new double[size];
      for(int i = 0; i < size; i++){
        std::cout << "item[" << i << "]";
        std::cin >> *(items + i);
      }
      iptr = items;
      break;
    case STRING:
      std::string *items = new std::string[size];
      for(int i = 0; i < size; i++){
        std::cout << "item[" << i << "]";
        std::cin >> *(items + i);
      }
      iptr = items;
      break;
    case ARRAY:
      Array *items = new Array[size];
      for(int i = 0; i < size; i++){
        std::cout << "item[" << i << "]";
        *(items + i) = createArr();
      }
      iptr = items;
      break;
  }
  newArray -> items = i;
  
  return newArray;
}

void inputValue(Database &database, std::string key, Type type){
  void *value;
  switch(type){
    case INT:
      value = new int;
      std::cin >> *(int *)value;
      break;
    case DOUBLE:
      value = new double;
      std::cin >> *(double *)value;
      break;
    case STRING:
      value = new std::string;
      std::cin.ignore();
      std::getline(std::cin, *(std::string *)value);
      break;
    case ARRAY:
      value = createArr();
      break;
  }
  add(database, create(type, key, value));
}

void getValue(Database database, std::string key){
  Entry *entry = get(database, key);
  Type type = entry -> type;
  void *value = entry -> value;
  switch(type){
    case INT:
      std::cout << entry -> key + ": " << *(int *)value << std::endl;
      break;
    case DOUBLE:
      std::cout << entry -> key + ": " << *(double *)value << std::endl;
      break;
    case STRING:
      std::cout << entry -> key + ": " << *(std::string *)value << std::endl;
      break;
    case ARRAY:
      // std::cout << entry -> key + ": " << *(Array *)value << std::endl;
      break;
  }
}

void dbStart(){
  int size = 0;
  Database database;
  init(database);
  while(true){
    std::string command;
    std::string type;
    std::string key;
    std::cout << "command (list, add, get, del, exit): ";
    std::cin >> command;
    if(command == "add"){
      std::cout << "key: ";
      std::cin >> key;
      std::cout << "type (int, double, string, array): ";
      std::cin >> type;
      std::cout << "value: ";
      inputValue(database, key, selectType(type));
      
      // if(type == "array"){
      //  = createArr();
      // }
      size += 1;
    }
    else if(command == "get"){
      std::cout << "key: ";
      std::cin >> key;
      getValue(database, key);
    }
    else if(command == "del"){
      std::cout << "key: ";
      std::cin >> key;
      remove(database, key);
    }
    else if(command == "exit"){
      destroy(database);
      break;
    }
  }
}