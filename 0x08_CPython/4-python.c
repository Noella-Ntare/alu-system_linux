#include <Python.h>
#include <unicodeobject.h>
#include <stdio.h>

/**
 * print_python_string - provides basic information about Python string objects
 * @p: Python Object
 * Author: Frank Onyema Orji
 * Return: void
 */
void print_python_string(PyObject *p)
{
    const char *type = NULL;
    Py_ssize_t length = 0;
    const char *value = NULL;

    printf("[.] string object info\n");
    if (!PyUnicode_Check(p))
    {
        printf("  [ERROR] Invalid String Object\n");
        fflush(stdout);
        return;
    }

    if (PyUnicode_IS_COMPACT_ASCII(p))
    {
        type = "compact ascii";
    }
    else if (PyUnicode_IS_COMPACT(p))
    {
        type = "compact unicode object";
    }
    else
    {
        type = "unicode object";
    }

    length = PyUnicode_GET_LENGTH(p);
    value = PyUnicode_AsUTF8AndSize(p, &length);

    printf("  type: %s\n", type);
    printf("  length: %ld\n", PyUnicode_GET_LENGTH(p));
    printf("  value: %s\n", value ? value : "");
    fflush(stdout);
}
