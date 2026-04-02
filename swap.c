/* a function to swap two values (used in sorting). */
#include <stdio.h>

void swap(double *a, double *b)
{
  double temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

int main(void)
{
  /* testing function */
  double x = 1.0, y = 2.0;
  printf("Before swap: x = %lf, y = %lf\n", x, y);
  swap(&x, &y);
  printf("After swap: x = %lf, y = %lf\n", x, y);

  return 0;
}
