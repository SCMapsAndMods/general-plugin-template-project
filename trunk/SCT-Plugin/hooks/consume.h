//The header file for the Consume hook module.
#pragma once
#include <SCBW/structures/CUnit.h>

namespace hooks {

//This function is injected into StarCraft.
void consumeHitHook(CUnit *target, CUnit* caster);

//The injector function for this hook module.
void injectConsumeHooks();

} //hooks
