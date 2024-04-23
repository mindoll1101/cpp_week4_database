#include <string>

enum Type { INT, DOUBLE, STRING, ARRAY };

struct Array {
  int size;
  Type type;
  void *items;
};

struct Entry {
  Type type;
  std::string key;
  void *value;
};

struct Database {
  int dbSize;
  Entry *db;
};

// 엔트리를 생성한다.
Entry *create(Type type, std::string key, void *value){
  Entry *newEntry = new Entry;
  newEntry -> type = type;
  newEntry -> key = key;
  switch(type){
    case INT:
      (newEntry -> value) = (int *)value;
      return newEntry;
    case DOUBLE:
      (newEntry -> value) = (double *)value;
      return newEntry;
    case STRING:
      (newEntry -> value) = (std::string *)value;
      return newEntry;
    case ARRAY:
      (newEntry -> value) = (Array *)value;
      return newEntry;
  }
  
};

// 데이터베이스를 초기화한다.
void init(Database &database){
  database.dbSize = 0;
  database.db = new Entry[database.dbSize];
};

// 데이터베이스에 엔트리를 추가한다.
void add(Database &database, Entry *entry){
  database.dbSize += 1;
  Entry *temp = database.db;
  database.db = new Entry[database.dbSize];
  for(int i = 0; i < database.dbSize - 1; i++){
    *(database.db + i) = *(temp + i);
  }
  database.db[database.dbSize - 1] = *entry;
  delete[] temp;
};

// 데이터베이스에서 키에 해당하는 엔트리를 찾는다.
Entry *get(Database &database, std::string &key){
  Entry *getEntry;
  for(int i = 0; i < database.dbSize; i++){
    if((database.db + i) -> key == key){
      return database.db + i;
    }
  }
  return NULL;
};

// 데이터베이스에서 키에 해당하는 엔트리를 제거한다.
void remove(Database &database, std::string &key){
  Entry *entry = get(database, key);
  
  if(entry != NULL){
    Entry *temp = database.db;
    int j = 0;
    
    database.db = new Entry[database.dbSize - 1];
    for(int i = 0; i < database.dbSize; i++){
      //key에 해당하는 entry만 제외하고 db에 복사
      if((temp + i) -> key != key){
        *(database.db + j) = *(temp + i);
        j++;
      }
    }
    database.dbSize--;
    delete entry;
    delete[] temp;
  }
};

// 데이터베이스를 해제한다.
void destroy(Database &database){
  delete[] database.db;
};
