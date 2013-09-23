#pragma once
#include "../SCBW/structures/CUnit.h"

namespace hooks {

void transferResourceToWorkerHook(CUnit *worker, CUnit *resource);

void harvestResourceInject();

} //hooks
