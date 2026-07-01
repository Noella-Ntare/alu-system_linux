#include <Python.h>
#include <stdio.h>
#include <listobject.h>
#include <bytesobject.h>
#include <object.h>

/**
 * print_python_bytes - provides basic information about Python bytes objects
 * @p: Python Object
 * Author: Frank Onyema Orji
 * Return: void
 */
void print_python_bytes(PyObject *p)
{
    PyBytesObject *pb = (PyBytesObject *)p;

    printf("[.] bytes object info\n");
    if (PyBytes_Check(p))
    {
        printf("  size: %ld\n", pb->ob_base.ob_size);
        printf("  trying string: %s\n", pb->ob_sval);
        if (pb->ob_base.ob_size < 10)
            printf("  first %ld bytes:", pb->ob_base.ob_size + 1);
        else
            printf("  first 10 bytes:");

        for (int i = 0; i <= pb->ob_base.ob_size && i < 10; ++i)
        {
            printf(" %02hhx", pb->ob_sval[i]);
        }
        printf("\n");
    }
    else
    {
        printf("  [ERROR] Invalid Bytes Object\n");
    }
    fflush(stdout);
}

/**
 * print_python_list - provides basic information about Python lists
 * @p: Python Object
 *
 * Return: void
 */
void print_python_list(PyObject *p)
{
    PyListObject *list = (PyListObject *)p;
    Py_ssize_t i;

    printf("[*] Python list info\n");
    printf("[*] Size of the Python List = %ld\n", list->ob_base.ob_size);
    printf("[*] Allocated = %ld\n", list->allocated);

    for (i = 0; i < list->ob_base.ob_size; i++)
    {
        printf("Element %ld: %s\n", i, list->ob_item[i]->ob_type->tp_name);

        if (PyBytes_Check(list->ob_item[i]))
        {
            print_python_bytes(list->ob_item[i]);
        }
    }
}
