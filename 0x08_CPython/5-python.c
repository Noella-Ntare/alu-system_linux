#include <stdio.h>
#include <stdbool.h>
#include <Python.h>
#include <longobject.h>

/**
 * print_python_int - Displays information about Python integer objects
 *
 * @p: Pointer to a PyObject struct
 * Author: Frank Onyema Orji
 */
void print_python_int(PyObject *p)
{
    PyVarObject *var = (PyVarObject *)p;
    PyLongObject *long_obj = (PyLongObject *)p;
    Py_ssize_t size;
    bool negative = false;
    unsigned long result = 0UL;
    unsigned long base = 1UL << PyLong_SHIFT;
    unsigned long digit;

    if (!PyLong_Check(p))
    {
        puts("Invalid Int Object");
        return;
    }

    size = var->ob_size;
    if (size < 0)
    {
        negative = true;
        size = -size;
    }

    for (Py_ssize_t i = 0; i < size; i++)
    {
        digit = (unsigned long)long_obj->ob_digit[i];

        if (result > ULONG_MAX / base ||
            (result == ULONG_MAX / base && digit > ULONG_MAX % base))
        {
            puts("C unsigned long int overflow");
            return;
        }

        result = result * base + digit;
    }

    if (negative)
        printf("-%lu\n", result);
    else
        printf("%lu\n", result);
}
