#include "web.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Callback pour `libcurl` (utilisé dans `web_search_agent_execute`)
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    WebContext* ctx = (WebContext*)userp;

    if (ctx->result == NULL) {
        ctx->result = malloc(1);
        ctx->result[0] = '\0';
    }

    ctx->result = realloc(ctx->result, strlen(ctx->result) + realsize + 1);
    memmove(ctx->result + strlen(ctx->result), contents, realsize);
    ctx->result[strlen(ctx->result) + realsize] = '\0';
    return realsize;
}

// Implémentation de la fonction (doit correspondre à la déclaration dans `web.h`)
void web_search_agent_execute(void* context) {
    WebContext* ctx = (WebContext*)context;
    CURL* curl = curl_easy_init();
    if (curl) {
        char* encoded_query = curl_easy_escape(curl, ctx->query, 0);
        char url[512];
        snprintf(url, sizeof(url), "https://api.duckduckgo.com/?q=%s&format=json&no_redirect=1", encoded_query);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, ctx);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Erreur curl: %s\n", curl_easy_strerror(res));
        }

        curl_free(encoded_query);
        curl_easy_cleanup(curl);
    }
}

