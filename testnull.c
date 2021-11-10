#include "types.h"
#include "stat.h"
#include "user.h"
#define NULL ((void *)0)

int main(int argc, char* argv[])
{
int a, b = 0, c = 0; // some integers
int *pi;     // a pointer to an integer
a = b + c;
a = 5;
pi = &a; // pi points to a
b = *pi; // b is now 5
pi = NULL;
c = *pi; // this is a NULL pointer dereference

printf(1, "Null Pointer Value: %p\n", *pi);
exit();
}
