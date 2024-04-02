#include "database.h"
#include <iostream>
#include <string>

Type selectType(std::string type){  //입력받은 타입을 enum으로 지정한다.
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

void createArr(void *array, bool *typeValid){    //arr포인터에 Array형 포인터 변수를 저장한다. array를 만든다.
  std::string type;
  Type t;
  std::cout << "type (int, double, string, array): ";
  std::getline(std::cin, type);
  if(type != "int" && type != "double" && type != "string" && type != "array"){
    std::cout << "No such type." << std::endl;
    *typeValid = false;
    return;
  }
  t = selectType(type);
  int size;
  void *items;
  std::cout << "size: ";
  std::cin >> size;
  std::cin.ignore();
  ((Array *)array) -> type = t;
  ((Array *)array) -> size = size;
  switch(t){
    case INT:
      items = new int[size];
      for(int i = 0; i < size; i++){
        std::cout << "item[" << i << "]: ";
        std::cin >> *((int *)items + i);    //items + i 주소의 변수에 값을 입력받음.
      }
      std::cin.ignore();
      ((Array *)array) -> items = items;
      break;
    case DOUBLE:
      items = new double[size];
      for(int i = 0; i < size; i++){
        std::cout << "item[" << i << "]: ";
        std::cin >> *((double *)items + i);
      }
      std::cin.ignore();
      ((Array *)array) -> items = items;
      break;
    case STRING:
      items = new std::string[size];
      for(int i = 0; i < size; i++){
        std::cout << "item[" << i << "]: ";
        std::getline(std::cin, *((std::string *)items + i));
      }
      ((Array *)array) -> items = items;
      break;
    case ARRAY:
      items = new Array[size];
      for(int i = 0; i < size; i++){
        std::cout << "item[" << i << "]: ";
        createArr((Array *)items + i, typeValid);
      }
      ((Array *)array) -> items = items;
      break;
  }
}

void inputValue(Database &database, std::string key, Type type){
  void *value;
  bool typeValid = true;
  switch(type){
    case INT:
      value = new int;
      std::cin >> *(int *)value;
      std::cin.ignore();
      break;
    case DOUBLE:
      value = new double;
      std::cin >> *(double *)value;
      std::cin.ignore();
      break;
    case STRING:
      value = new std::string;
      std::getline(std::cin, *(std::string *)value);
      break;
    case ARRAY:
      value = new Array;
      createArr(value, &typeValid);
      break;
  }
  if(typeValid)
    add(database, create(type, key, value));
}

void getValue(void *value, Type type){
  switch(type){
    case INT:
      std::cout << *(int *)value;
      break;
    case DOUBLE:
      std::cout << *(double *)value;
      break;
    case STRING:
      std::cout << "\"" <<*(std::string *)value << "\"";
      break;
    case ARRAY:
      std::cout << "[";
      switch(((Array *)value) -> type){
        case INT:
          for(int i = 0; i < ((Array *)value) -> size; i++){
            getValue(((int *)(((Array *)value) -> items) + i), ((Array *)value) -> type);
            if(i != ((Array *)value) -> size - 1){
              std::cout << ", ";
            }
          }
          break;
        case DOUBLE:
        for(int i = 0; i < ((Array *)value) -> size; i++){
            getValue(((double *)(((Array *)value) -> items) + i), ((Array *)value) -> type);
            if(i != ((Array *)value) -> size - 1){
              std::cout << ", ";
            }
          }
          break;
        case STRING:
          for(int i = 0; i < ((Array *)value) -> size; i++){
            getValue(((std::string *)(((Array *)value) -> items) + i), ((Array *)value) -> type);
            if(i != ((Array *)value) -> size - 1){
              std::cout << ", ";
            }
          }
          break;
        case ARRAY:
          for(int i = 0; i < ((Array *)value) -> size; i++){
            getValue(((Array *)(((Array *)value) -> items) + i), ((Array *)value) -> type);
            if(i != ((Array *)value) -> size - 1){
              std::cout << ", ";
            }
          }
          break;
      }
      std::cout << "]";
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
    std::getline(std::cin, command);
    if(command == "add"){
      std::cout << "key: ";
      std::getline(std::cin, key);
      std::cout << "type (int, double, string, array): ";
      std::getline(std::cin, type);
      if(type != "int" && type != "double" && type != "string" && type != "array"){
        std::cout << "No such type." << std::endl;
        continue;
      }
      std::cout << "value: ";
      inputValue(database, key, selectType(type));
      
      size += 1;
    }
    else if(command == "get"){
      std::cout << "key: ";
      std::getline(std::cin, key);
      Entry *entry = get(database, key);
      if(entry == NULL){
        std::cout << "Not Found" << std::endl;
      }
      else{
        Type type = entry -> type;
        void *value = entry -> value;
        std::cout << entry -> key + ": ";
        getValue(value, type);
        std::cout << std::endl;
      }
    }
    else if(command == "del"){
      std::cout << "key: ";
      std::getline(std::cin, key);
      remove(database, key);
    }
    else if(command == "exit"){
      destroy(database);
      break;
    }
    else if(command == "list"){
      if(database.dbSize < 1){
        std::cout << "Empty database." << std::endl;
      }
      else{
        for(int i = 0; i < database.dbSize; i++){
          std::cout << (database.db + i) -> key + ": ";
          getValue((database.db + i) -> value, (database.db + i) -> type);
          std::cout << std::endl;
        }
      }
    }
    else{
      std::cout << "Invalid command. Try again." << std::endl;
    }
  }
}
