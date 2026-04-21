#include "export.h"
#include <sys/stat.h>

// 2d array of links to set of cubes (+ last row for left cubes)
Cubeset** init_export_data(int ci_size) {
    Cubeset** arr = calloc(sizeof(Cubeset*), ci_size + 1); // +1 for zi
    for (int i = 0; i <= ci_size; i++)
        arr[i] = calloc(sizeof(Cubeset), ci_size);

    // calloc = everything is filled with 0 = NULL = EMPTY_SET
    return arr;
}

void export_data_free(Cubeset** data, int ci_size) {
    for (int i = 0; i <= ci_size; i++) {
        for (int j = 0; j < ci_size; j++)
            set_free(data[i][j]);
        free(data[i]);
    }
    free(data);
}

void export_csv(Cubeset ci, Cubeset** data, const char* test_path, int iteration, time_t* now, const char* prefix, const char* name, const char* left_name) {
    char filename[FILENAME_MAX] = {0}; // think about better way of manipulating strings (slices?)
    strcat(filename, test_path);
    strcat(filename + strlen(test_path), "_results/");
    mkdir(filename, 0777);
    strcat(filename + strlen(test_path) + 9, prefix);
    sprintf(filename + strlen(test_path) + strlen(prefix) + 9, "%02d", iteration);
    char *filename_time_ptr = get_time_formated(filename + strlen(test_path) + strlen(prefix) + 9, now);
    strcat(filename_time_ptr, ".csv");
    FILE *file = fopen(filename, "w");

    fprintf(file, "\"%s\",", name);
    for (int i = 0; i < size(ci); i++) {
        fprintf(file, "\"");
        cube_print(file, ci[i], NULL);
        fprintf(file, "\"");
        if (i < size(ci) - 1) fprintf(file, ",");
    }
    fprintf(file, "\n");

    for (int i = 0; i <= size(ci); i++) {
        fprintf(file, "\"");
        if (i < size(ci))
            cube_print(file, ci[i], NULL);
        else
            fprintf(file, "%s", left_name);
        fprintf(file, "\"");
        fprintf(file, ",");
        for (int j = 0; j < size(ci); j++) {
            if (i == j) {
                fprintf(file, "\"----\"");
            } else {
                fprintf(file, "\"");
                // if (i == size(ci) && is_empty(data[i][j]))
                //     fprintf(file, "Ø");
                for (int k = 0; k < size(data[i][j]); k++) {
                    cube_print(file, data[i][j][k], NULL);
                    if (k < size(data[i][j]) - 1) fprintf(file, "\n");
                }
                fprintf(file, "\"");
            }
            if (j < size(ci) - 1) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }


    fclose(file);
}
