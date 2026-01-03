#ifndef AGENT_H
#define AGENT_H

typedef struct {
    char* name;
    void (*execute)(void* context);
    void* context;
} Agent;

Agent* create_agent(const char* name, void (*execute)(void* context), void* context);
void destroy_agent(Agent* agent);

#endif
