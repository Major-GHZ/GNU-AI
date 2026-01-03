#include "agent.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Agent* create_agent(const char* name, void (*execute)(void* context), void* context) {
    Agent* agent = (Agent*)malloc(sizeof(Agent));
    agent->name = strdup(name);
    agent->execute = execute;
    agent->context = context;
    return agent;
}

void destroy_agent(Agent* agent) {
    free(agent->name);
    free(agent);
}
