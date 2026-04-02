#include <stdio.h>

void swap(double *a, double *b)
{
  double temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

void bubbleSort(double arr[], int size)
{
  int sorted;
  do
  {
    sorted = 1;
    for (int i = 0; i < size - 1; i++)
    {
      if (arr[i] > arr[i + 1])
      {
        swap(&arr[i], &arr[i + 1]);
        sorted = 0;
      }
    }
  } while (!sorted);
}

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
a function to calculate the quartiles. In this function, (1) call the function bubbleSort(double
arr[], int size) to sort the array; (2) call the function findMedian(double arr[], int start, int end) to
find the median of the sorted array as Q2; (3) call the function findMedian(double arr[], int start, int
end) to find the median of the lower half of the sorted array as Q1 and the median of the upper half
as Q3.
*/
void findQuartiles(double arr[], int size, double *Q1, double *Q2, double *Q3)
{
  bubbleSort(arr, size);
  *Q2 = findMedian(arr, 0, size - 1);
  if (size % 2 == 0)
  {
    *Q1 = findMedian(arr, 0, size / 2 - 1);
    *Q3 = findMedian(arr, size / 2, size - 1);
  }
  else
  {
    *Q1 = findMedian(arr, 0, (size - 1) / 2 - 1);
    *Q3 = findMedian(arr, (size - 1) / 2 + 1, size - 1);
  }
}

int main(void)
{
  double example_arr[] = {0.98, 1.17, 1.6, 1.77, 2.03, 2.13, 2.47, 2.49, 2.53, 2.59, 2.74, 2.75, 3.04, 3.14, 3.2, 3.21,
                          3.23, 3.23, 3.29, 3.29, 3.57, 3.58, 3.61, 3.66, 3.73, 3.88, 3.89, 3.94, 3.95, 4.45};
  int size = 30;
  double Q1, Q2, Q3;
  findQuartiles(example_arr, size, &Q1, &Q2, &Q3);
  printf("Q1: %lf\n", Q1);
  printf("Q2 (Median): %lf\n", Q2);
  printf("Q3: %lf\n", Q3);
  
  return 0;
}
