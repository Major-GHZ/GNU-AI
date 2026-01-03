#ifndef WEB_H
#define WEB_H

#include <curl/curl.h>

typedef struct {
    char* query;
    char* result;
} WebContext;

// Déclaration de la fonction (pour que le compilateur sache qu'elle existe)
void web_search_agent_execute(void* context);

#endif

