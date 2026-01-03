// include/memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include <sqlite3.h>

typedef struct {
    sqlite3 *db;
    char *user_id;
} Memory;

Memory* init_memory(const char *user_id);
void save_conversation(Memory *mem, const char *prompt, const char *response);
char* load_last_conversations(Memory *mem, int limit);
void close_memory(Memory *mem);

#endif
