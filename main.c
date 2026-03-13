#include <stdio.h>
#include <time.h>

#include "checker.h"
#include "rots_algorithm.h"

int main(void) {
    // Set *mdnf = rots_algorithm("tests/test1.txt");
    // bool correct = verify_function("tests/test1.txt", mdnf);
    //
    // if (correct) printf("Correct!\n");
    // else printf("Truth tables are not the same!\n");
    //
    // set_free_ptr(mdnf);

    srand(time(nullptr));
    int tests_count = 100;
    while (tests_count--) {
        generate_test("tests/generated_test.txt");
        Set *mdnf = rots_algorithm("tests/generated_test.txt");
        bool correct = verify_function("tests/generated_test.txt", mdnf);

        set_free_ptr(mdnf);

        if (correct) printf("Correct!\n");
        else {
            printf("Truth tables are not the same!\n");
            return 0;
        }

        printf("\n");
    }

    return 0;
}

// todo implement on zero values
// todo Set operations in Ci*Ci is the bottleneck for big tests
