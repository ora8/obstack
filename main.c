#include <stdio.h>
#include <string.h>

#include "obstack.h"

int main() {
    struct obstack obstack1;
    obstack_init(&obstack1, 1024); // Chunk-Größe: 1024 Bytes

    // Allokiere einige Objekte
    int *x = obstack_alloc(&obstack1, sizeof(int));
    *x = 42;

    char *str = obstack_alloc(&obstack1, 20);
    strcpy(str, "Hello, Obstack!");

    double *y = obstack_alloc(&obstack1, sizeof(double));
    *y = 3.14159;

    printf("x = %d\n", *x);
    printf("str = %s\n", str);
    printf("y = %f\n", *y);

    // Alle Objekte freigeben
    obstack_free(&obstack1);

    return 0;
}
