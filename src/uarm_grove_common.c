//
// Created by cap on 4/21/20.
//

#include "uarm_grove_common.h"

GroveModule_t* newGroveModule(GroveType_t moduleType)
{
    DB_PRINT_STR("New Grove module of type: %d\r\n", moduleType);
    GroveModule_t *m = malloc(sizeof(*m));
    m->_moduleType = moduleType;
    return m;
}

void delGroveModule(GroveModule_t* module)
{
    DB_PRINT_STR("Free memory for module\r\n");
    free((void *)module);
}

