#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
int a;
int b;
int thread_code(void *arg)
{
int c;
b=a+10;
c=a+20;
}
int main()
{
int c=3;
a=1;
b=2;
void *p;
p=(void *)malloc(16384);
p+=16383;
clone(thread_code,p,CLONE_VM,0);
for (int i=0;i<=2;i++){
printf("a=%d, b=%d, c=%d\n", a, b, c);
}
}