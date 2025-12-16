#include <stdio.h>
#include <stdlib.h>

static int merge_count_calls = 0;
static int quicksort_partition_calls = 0;
static int bubble_comparisons = 0;
static int insertion_comparisons = 0;
static int selection_comparisons = 0;

int merge_count(int a[], int l, int m, int r)
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
      merge_count_calls++;
   }

   while (i < n1) {
      a[k++] = left[i++];
      merge_count_calls++;
   }
   while (j < n2) {
      a[k++] = right[j++];
      merge_count_calls++;
   }

   free(left);
   free(right);
   return merge_count_calls;
}

int merge_sort_count(int a[], int l, int r)
{
   if (l < r)
   {
      int m = l + (r - l) / 2;
      int left_iter = merge_sort_count(a, l, m);
      int right_iter = merge_sort_count(a, m + 1, r);
      int merge_iter = merge_count(a, l, m, r);
      return left_iter + right_iter + merge_iter;
   }
   return 0;
}

int partition_count(int a[], int l, int r)
{
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
      quicksort_partition_calls++;
   }
   a[r] = a[i];
   a[i] = p;
   return quicksort_partition_calls;
}

int quicksort_count(int a[], int l, int r)
{
   if (l >= r)
      return 0;
      
   int before_calls = quicksort_partition_calls;
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
      quicksort_partition_calls++;
   }
   a[r] = a[i];
   a[i] = p;
   
   int left_iter = quicksort_count(a, l, i - 1);
   int right_iter = quicksort_count(a, i + 1, r);
   
   return quicksort_partition_calls - before_calls + left_iter + right_iter;
}

int bubble_sort_count(int a[], int n)
{
   int comparisons = 0;
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
         comparisons++;
      }
   }
   return comparisons;
}

int insertion_sort_count(int a[], int n)
{
   int comparisons = 0;
   for (int i = 1; i < n; i++)
   {
      int key = a[i];
      int j = i - 1;
      while (j >= 0 && a[j] > key)
      {
         a[j + 1] = a[j];
         j--;
         comparisons++;
      }
      if (j >= 0) comparisons++; 
      a[j + 1] = key;
   }
   return comparisons;
}

int selection_sort_count(int a[], int n)
{
   int comparisons = 0;
   for (int i = 0; i < n - 1; i++)
   {
      int min_idx = i;
      for (int j = i + 1; j < n; j++)
      {
         if (a[j] < a[min_idx]) {
            min_idx = j;
         }
         comparisons++;
      }
      int temp = a[i];
      a[i] = a[min_idx];
      a[min_idx] = temp;
   }
   return comparisons;
}


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