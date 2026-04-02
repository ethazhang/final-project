#include <stdio.h>

double findMedian(double arr[], int start, int end)
{
  int size = end - start + 1;
  if (size % 2 == 1)
  {
    return arr[start + size / 2];
  }
  else
  {
    return (arr[start + size / 2 - 1] + arr[start + size / 2]) / 2.0;
  }
}

/*
a function to find the median of an array. Median calculation follows: (1) if odd number of
elements → Middle value is Q2; (2) if even number of elements → Average of two middle values. For
sorted L = [0.98, 1.17, 1.6, 1.77, 2.03, 2.13, 2.47, 2.49, 2.53, 2.59, 2.74, 2.75, 3.04, 3.14, 3.2, 3.21,
3.23, 3.23, 3.29, 3.29, 3.57, 3.58, 3.61, 3.66, 3.73, 3.88, 3.89, 3.94, 3.95, 4.45], total number is 30
which is an even number such that median is the average of the 15th and 16th numbers (3.205 =
(3.2 + 3.21) / 2).
*/
int main(void)
{
  double example_arr[] = {0.98, 1.17, 1.6, 1.77, 2.03, 2.13, 2.47, 2.49, 2.53, 2.59, 2.74, 2.75, 3.04, 3.14, 3.2, 3.21,
                          3.23, 3.23, 3.29, 3.29, 3.57, 3.58, 3.61, 3.66, 3.73, 3.88, 3.89, 3.94, 3.95, 4.45};
  double median = findMedian(example_arr, 0, 29);
  printf("Median: %lf\n", median);

  return 0;
}
