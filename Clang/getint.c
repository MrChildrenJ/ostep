#include <ctype.h>
#include <stdio.h>
#define BUFSIZE 100

static char buf[BUFSIZE];
static int bufp = 0;

int getch(void)     { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch(int c) {
    if (bufp >= BUFSIZE)    printf("ungetch: too many characters\n");
    else                    buf[bufp++] = c;
}

int getint(int* pn) {
    int c, sign;

    while (isspace(c = getch()));

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-')   c = getch();

    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');
    *pn *= sign;

    if ( c != EOF)  ungetch(c);
    return c;
}

int main() {
    int n, result;
    printf("Enter a number: ");

    while ((result = getint(&n)) != EOF) {
        if (result == 0) {
            printf("Not a number\n");
            break;
        }
        else {
            printf("Number: %d\n", n);
            printf("Enter a number: ");
        }
    }
    return 0;
}