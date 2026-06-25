#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/**
 * prime_factors - factorizes a number into a list of prime factors
 * @s: string number
 * Return: pointer to list of prime factors
 */
list_t *prime_factors(char const *s)
{
unsigned long num, divisor, *factor;
char *end;
list_t *list;
num = strtoul(s, &end, 10);
if (*end != '\0')
return (NULL);
list = malloc(sizeof(list_t));
list_init(list);
divisor = 2;
while (divisor *divisor <= num)
{
while (num % divisor == 0)
{
factor = malloc(sizeof(unsigned long));
*factor = divisor;
list_add(list, factor);
num /= divisor;
}
divisor += 1 + (divisor != 2);
}
if (num >= 2)
{
factor = malloc(sizeof(unsigned long));
*factor = num;
list_add(list, factor);
}
return (list);
}
