#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int pid = fork();
    printf("Hello from process %d\n", pid);
    return 0;
}