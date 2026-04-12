#pragma once
#include "cubeset.h"

Cubeset dead_ends_finding_tree(Cubeset not_E, Cubeset not_E_covered);
Cubeset* dead_ends_finding_bruteforce(Cubeset not_E, Cubeset not_E_covered);