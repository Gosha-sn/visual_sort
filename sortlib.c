
#include <stdio.h>
#include <stdlib.h>

void merge(int a[], int l, int m, int r)
{
   int n1 = m - l + 1;
   int n2 = r - m;
   int *left = malloc(n1 * sizeof(int));
   int *right = malloc(n2 * sizeof(int));
   for (int i = 0; i < n1; i++)
      left[i] = a[l + i];
   for (int j = 0; j < n2; j++)
      right[j] = a[m + 1 + j];

   int i = 0, j = 0, k = l;
   while (i < n1 && j < n2)
   {
      if (left[i] <= right[j])
         a[k++] = left[i++];
      else
         a[k++] = right[j++];
   }

   while (i < n1)
      a[k++] = left[i++];
   while (j < n2)
      a[k++] = right[j++];

   free(left);
   free(right);
}
void merge_sort(int a[], int l, int r)
{
   if (l < r)
   {
      int m = l + (r - l) / 2;
      merge_sort(a, l, m);
      merge_sort(a, m + 1, r);
      merge(a, l, m, r);
   }
}
void quicksort(int a[], int l, int r)
{
   if (l >= r)
      return;
   int p = a[r];
   int i = l;
   for (int j = l; j < r; j++)
   {
      if (a[j] <= p)
      {
         int t = a[i];
         a[i] = a[j];
         a[j] = t;
         i++;
      }
   }
   a[r] = a[i];
   a[i] = p;
   quicksort(a, l, i - 1);
   quicksort(a, i + 1, r);
}
void bubble_sort(int a[], int n)
{
   for (int i = 0; i < n - 1; i++)
   {
      for (int j = 0; j < n - 1 - i; j++)
      {
         if (a[j] > a[j + 1])
         {
            int temp = a[j];
            a[j] = a[j + 1];
            a[j + 1] = temp;
         }
      }
   }
}

void insertion_sort(int a[], int n)
{
   for (int i = 1; i < n; i++)
   {
      int key = a[i];
      int j = i - 1;
      while (j >= 0 && a[j] > key)
      {
         a[j + 1] = a[j];
         j--;
      }
      a[j + 1] = key;
   }
}
void selection_sort(int a[], int n)
{
   for (int i = 0; i < n - 1; i++)
   {
      int min_idx = i;
      for (int j = i + 1; j < n; j++)
      {
         if (a[j] < a[min_idx])
            min_idx = j;
      }
      int temp = a[i];
      a[i] = a[min_idx];
      a[min_idx] = temp;
   }
}