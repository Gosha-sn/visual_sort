#ifndef SORTLIB_H
#define SORTLIB_H

void merge(int a[], int l, int m, int r);
void merge_sort(int a[], int l, int r);
int merge_sort_count(int a[], int l, int r);

void quicksort(int a[], int l, int r);
int quicksort_count(int a[], int l, int r);

void bubble_sort(int a[], int n);
int bubble_sort_count(int a[], int n);

void insertion_sort(int a[], int n);
int insertion_sort_count(int a[], int n);

void selection_sort(int a[], int n);
int selection_sort_count(int a[], int n);

#endif