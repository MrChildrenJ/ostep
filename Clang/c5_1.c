#include <ctype.h>
#include <stdio.h>
#define BUFSIZE 100

static char buf[BUFSIZE];
static int  bufp = 0;

int getch(void)     { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch(int c) {
    if (bufp >= BUFSIZE)    printf("ungetch: too many characters\n");
    else                    buf[bufp++] = c;
}

int getint(int* pn) {
    int c, sign;

    while (isspace(c = getch()));

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {  // Not digit, +, -, or EOF
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c == '-') {
        c = getch();
        if (!isdigit(c)) {  // if "the c" is not a digit
            ungetch(c);     // push back "the c"
            // push the char before c, i.e. +/-
            ungetch((sign == -1) ? '-' : '+');  
            return 0;
        }
    }

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