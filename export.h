#pragma once
#include "cubeset.h"

Cubeset** init_export_data(int ci_size);
void export_data_free(Cubeset** data, int ci_size);

void export_csv(Cubeset ci, Cubeset** data, const char* test_path, int iteration, time_t* now, const char* prefix, const char* name, const char* left_name);