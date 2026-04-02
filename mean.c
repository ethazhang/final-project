#include <stdio.h>

/*
a function which computes the average of all the values in the array arr[]. The formula for the average is Σarr[i] /
size of arr[]. For example, the mean of the above 30 values (not counting the first value which means
day 1) is 2.97.
*/
double mean(double arr[], int size)
{
  double sum = 0.0;
  for (int i = 0; i < size; i++)
  {
    sum += arr[i];
  }
  return sum / size;
}

int main(void)
{
  double example_arr[] = {0.98, 1.17, 1.6, 1.77, 2.03, 2.13, 2.47, 2.49, 2.53, 2.59, 2.74, 2.75, 3.04, 3.14, 3.2, 3.21,
                          3.23, 3.23, 3.29, 3.29, 3.57, 3.58, 3.61, 3.66, 3.73, 3.88, 3.89, 3.94, 3.95, 4.45};
  int size = 30;
  double avg = mean(example_arr, size);
  printf("Mean: %lf\n", avg);
  return 0;
}
