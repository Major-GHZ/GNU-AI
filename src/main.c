#include <stdlib.h>
#include <stdio.h>
#include <string.h>  // <-- Pour strdup !
#include "agent.h"
#include "web.h"

int main() {
    // Une seule déclaration de ctx, avec initialisation propre
    WebContext ctx = {
        .query = "doc sur les LLM",
        .result = strdup("")  // Initialisation sécurisée
    };

    Agent* web_agent = create_agent("WebSearchAgent", web_search_agent_execute, &ctx);
    web_agent->execute(web_agent->context);

    if (ctx.result != NULL) {  // Vérification de sécurité
        printf("Résultat de la recherche: %s\n", ctx.result);
        free(ctx.result);  // Libération propre
    }

    destroy_agent(web_agent);
    return 0;
}

