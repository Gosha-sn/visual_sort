#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
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

double merge_sort_count(int a[], int l, int r)
{
   clock_t start = clock();
   if (l < r)
   {
      int m = l + (r - l) / 2;
      int left_iter = merge_sort_count(a, l, m);
      int right_iter = merge_sort_count(a, m + 1, r);
      int merge_iter = merge_count(a, l, m, r);
      return left_iter + right_iter + merge_iter;
   }
   clock_t end = clock();
   double seconds = (double)(end - start) / CLOCKS_PER_SEC;
   return seconds;
}

double partition_count(int a[], int l, int r)
{
   clock_t start = clock();
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
   clock_t end = clock();
   double seconds = (double)(end - start) / CLOCKS_PER_SEC;
   return seconds;
}

double quicksort_count(int a[], int l, int r)
{
   clock_t start = clock();
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
   clock_t end = clock();
   double seconds = (double)(end - start) / CLOCKS_PER_SEC;
   return seconds;
}

double bubble_sort_count(int a[], int n)
{
   clock_t start = clock();
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
   clock_t end = clock();
   double seconds = (double)(end - start) / CLOCKS_PER_SEC;
   return seconds;
}

double insertion_sort_count(int a[], int n)
{
   clock_t start = clock();
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
   clock_t end = clock();
   double seconds = (double)(end - start) / CLOCKS_PER_SEC;
   return seconds;
}

double selection_sort_count(int a[], int n)
{
   clock_t start = clock();
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
   clock_t end = clock();
   double seconds = (double)(end - start) / CLOCKS_PER_SEC;
   return seconds;
}



// int main(){
//    srand(time(NULL));
//    int len = 100000;
//    int *test_list = malloc(len*sizeof(int));
//    for (int i =0; i<len;i++) {
//       test_list[i] = rand() % 100;
//    }
//    double resp =  selection_sort_count(test_list, len);
//    printf("%lf", resp);
// }