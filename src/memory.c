// src/memory.c
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Memory* init_memory(const char *user_id) {
    Memory *mem = malloc(sizeof(Memory));
    mem->user_id = strdup(user_id);

    // Ouvre ou crée la base de données
    int rc = sqlite3_open("data/users.db", &mem->db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(mem->db));
        exit(1);
    }

    // Crée la table si elle n'existe pas
    char *sql = "CREATE TABLE IF NOT EXISTS conversations("
                "user_id TEXT NOT NULL,"
                "prompt TEXT NOT NULL,"
                "response TEXT NOT NULL,"
                "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
    rc = sqlite3_exec(mem->db, sql, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(mem->db));
        exit(1);
    }

    return mem;
}

void save_conversation(Memory *mem, const char *prompt, const char *response) {
    char *sql = malloc(1024);
    sprintf(sql,
        "INSERT INTO conversations(user_id, prompt, response) "
        "VALUES('%s', '%s', '%s');",
        mem->user_id, prompt, response);

    int rc = sqlite3_exec(mem->db, sql, 0, 0, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(mem->db));

    free(sql);
}

char* load_last_conversations(Memory *mem, int limit) {
    char *sql = malloc(512);
    sprintf(sql,
        "SELECT prompt, response FROM conversations "
        "WHERE user_id = '%s' ORDER BY timestamp DESC LIMIT %d;",
        mem->user_id, limit);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(mem->db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch conversations: %s\n", sqlite3_errmsg(mem->db));
        return NULL;
    }

    char *result = malloc(4096);
    result[0] = '\0';
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char *prompt = (const char*)sqlite3_column_text(stmt, 0);
        const char *response = (const char*)sqlite3_column_text(stmt, 1);
        strcat(result, prompt);
        strcat(result, " -> ");
        strcat(result, response);
        strcat(result, "\n");
    }

    sqlite3_finalize(stmt);
    free(sql);
    return result;
}

void close_memory(Memory *mem) {
    sqlite3_close(mem->db);
    free(mem->user_id);
    free(mem);
}
