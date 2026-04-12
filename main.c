#include <stdio.h>
#include <time.h>
#include "rots_algorithm.h"
#include "checker.h"

#define HAND_TESTING

int main(void) {
#ifdef HAND_TESTING
    const char *test_filename = "tests/test5.txt";
    Cubeset* dead_ends = rots_algorithm(test_filename);

    for (int i = 0; i < size(dead_ends); i++) {
        printf("\n ====== DEAD END №%d ======\n", i + 1);

        bool correct = verify_function(test_filename, dead_ends[i]);

        if (correct) printf("Correct!\n");
        else printf("Truth tables are not the same!\n");

        set_free(dead_ends[i]);
    }

    set_free(dead_ends);
#  else
    srand(time(nullptr));
    int tests_count = 100;
    while (tests_count--) {
        generate_test("tests/generated_test.txt");
        Cubeset mdnf = rots_algorithm("tests/generated_test.txt");
        bool correct = verify_function("tests/generated_test.txt", mdnf);

        set_free(mdnf);

        if (correct) printf("Correct!\n");
        else {
            printf("Truth tables are not the same!\n");
            return 0;
        }

        printf("\n");
    }
#endif
    return 0;
}

// todo make webui

// todo implement on zero values
// todo i think Set operations in Ci*Ci is the bottleneck for big tests
