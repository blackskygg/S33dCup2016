#include <stdio.h>

void foo()
{
    int a, b = 1;

    a = 1;

    printf("output: %d\n", a++);

/*  a = 2
 *  b = 1
 */

    if (a > 1)
        a = b + 2 - 3 * 4 / 5;
    else if (a + b == 2)
        printf("output: %d\n", b++);
    else
        ;

    while (b, a <= 10) {
        int b = 2;
        printf("%d\n", a);
        a++;
    }

// a = 11, b = 2

    do
        b -= 1;
    while (b);

/* a = 11, b = 0 */

    for (int a = 4, b = 3, c = 5, d = 2; a + b * 2 < c + d; a = a - b, b++, c--)
        ;

    while (1)
        break;
}
