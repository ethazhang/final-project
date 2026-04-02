/*
a function to sort an array using Bubble Sort. Bubble Sort is a simple and beginner-
friendly sorting algorithm that repeatedly compares and swaps adjacent elements until the entire
list is sorted.
How Bubble Sort Works (Step-by-Step)
1. Start from the first element and compare it with the next element.
2. If the first element is greater, swap them.
3. Move to the next pair and repeat the process.
4. Continue this until the largest element moves to the end.
5. Repeat the process for the remaining unsorted portion of the list.
6. Continue until no swaps are needed.
*/
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

int main(void)
{
  double example_arr[] = {5, 3, 8, 1, 2};
  int size = 5;
  bubbleSort(example_arr, size);

  printf("Sorted array:\n");
  for (int i = 0; i < size; i++)
  {
    printf("%lf\n", example_arr[i]);
  }

  return 0;
}
