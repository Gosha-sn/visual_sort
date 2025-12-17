#ifndef SORTLIB_H
#define SORTLIB_H

int merge_count(int a[], int l, int m, int r);

double  merge_sort_count(int a[], int l, int r);


double quicksort_count(int a[], int l, int r);

void bubble_sort(int a[], int n);
double bubble_sort_count(int a[], int n);

void insertion_sort(int a[], int n);
double insertion_sort_count(int a[], int n);

void selection_sort(int a[], int n);
double selection_sort_count(int a[], int n);

#endif