#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "sortlib.h"

#define ID_COMBO_SORT 100
#define ID_INPUT 101
#define ID_GENERATE 102
#define ID_SORT 103
#define ID_OUTPUT 104

HINSTANCE hInst;
HWND hwndCombo, hwndInput, hwndOutput;
HBRUSH g_hBrush = NULL;
HBRUSH g_editBrush = NULL;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GenerateRandomList(HWND);
void SortList(HWND);
int ValidateInput(const char *input);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   srand(time(NULL));
   const char *szClassName = "MainWindow";
   hInst = hInstance;

   g_hBrush = CreateSolidBrush(RGB(40, 44, 52));

   WNDCLASSEX wc = {0};
   wc.cbSize = sizeof(WNDCLASSEX);
   wc.style = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc = WndProc;
   wc.hInstance = hInstance;
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = g_hBrush;
   wc.lpszClassName = szClassName;

   RegisterClassEx(&wc);

   HWND hwnd = CreateWindow(
       szClassName,
       "Sorting Algorithm Visualizer",
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
       NULL, NULL, hInstance, NULL);

   ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);

   MSG msg;
   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
   case WM_CREATE:
   {
      hwndCombo = CreateWindow(
          "COMBOBOX", "",
          CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
          10, 10, 200, 200,
          hwnd, (HMENU)ID_COMBO_SORT, hInst, NULL);
      SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM) "Merge Sort");
      SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM) "Quick Sort");
      SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM) "Bubble Sort");
      SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM) "Insertion Sort");
      SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM) "Selection Sort");
      SendMessage(hwndCombo, CB_SETCURSEL, 0, 0);

      hwndInput = CreateWindow(
          "EDIT", "",
          WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
          220, 10, 350, 25,
          hwnd, (HMENU)ID_INPUT, hInst, NULL);

      CreateWindow(
          "BUTTON", "Generate",
          WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
          10, 40, 80, 30,
          hwnd, (HMENU)ID_GENERATE, hInst, NULL);

      CreateWindow(
          "BUTTON", "Sort",
          WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
          100, 40, 80, 30,
          hwnd, (HMENU)ID_SORT, hInst, NULL);

      hwndOutput = CreateWindow(
          "EDIT", "",
          WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
          10, 80, 560, 280,
          hwnd, (HMENU)ID_OUTPUT, hInst, NULL);
      break;
   }

   case WM_CTLCOLORDLG:
   case WM_CTLCOLORBTN:
   case WM_CTLCOLOREDIT:
   case WM_CTLCOLORLISTBOX:
   case WM_CTLCOLORSCROLLBAR:
   case WM_CTLCOLORSTATIC:
   {
      HDC hdc = (HDC)wParam;
      HWND hwndCtl = (HWND)lParam;

      if (hwndCtl == hwndInput || hwndCtl == hwndOutput)
      {
         SetTextColor(hdc, RGB(220, 220, 220));
         SetBkColor(hdc, RGB(60, 65, 75));
         if (!g_editBrush)
            g_editBrush = CreateSolidBrush(RGB(60, 65, 75));
         return (INT_PTR)g_editBrush;
      }
      break;
   }

   case WM_COMMAND:
   {
      if (LOWORD(wParam) == ID_GENERATE)
      {
         GenerateRandomList(hwnd);
      }
      else if (LOWORD(wParam) == ID_SORT)
      {
         SortList(hwnd);
      }
      break;
   }

   case WM_DESTROY:
      if (g_hBrush)
         DeleteObject(g_hBrush);
      if (g_editBrush)
         DeleteObject(g_editBrush);
      PostQuitMessage(0);
      break;

   default:
      return DefWindowProc(hwnd, message, wParam, lParam);
   }
   return 0;
}

void GenerateRandomList(HWND hwnd)
{
   char buffer[256];
   int count = 10 + rand() % 10;

   strcpy(buffer, "");
   for (int i = 0; i < count; i++)
   {
      char num_str[16];
      sprintf(num_str, "%d", rand() % 100);
      strcat(buffer, num_str);
      if (i < count - 1)
         strcat(buffer, " ");
   }

   SetWindowText(hwndInput, buffer);
}

int ValidateInput(const char *input)
{
   if (!input || !*input)
      return 0;

   int in_number = 0;
   int has_space = 0;
   const char *p = input;

   while (*p)
   {
      if (isspace(*p))
      {
         if (in_number)
         {
            in_number = 0;
         }
      }
      else if (isdigit(*p) || (*p == '-' && isdigit(*(p + 1)) && (!p || isspace(*(p - 1)) || p == input)))
      {
         in_number = 1;
      }
      else
      {
         return 0;
      }
      p++;
   }

   return in_number;
}

void SortList(HWND hwnd)
{
   char input_buffer[1024];
   GetWindowText(hwndInput, input_buffer, sizeof(input_buffer));

   if (!ValidateInput(input_buffer))
   {
      MessageBox(hwnd, "Invalid input! Please enter only numbers separated by spaces.", "Error", MB_OK | MB_ICONERROR);
      return;
   }

   int arr[256];
   int n = 0;
   char *token = strtok(input_buffer, " ");
   while (token != NULL && n < 256)
   {
      arr[n++] = atoi(token);
      token = strtok(NULL, " ");
   }

   if (n == 0)
   {
      MessageBox(hwnd, "No valid numbers found!", "Error", MB_OK | MB_ICONERROR);
      return;
   }

   int temp_arr[256];
   for (int i = 0; i < n; i++)
   {
      temp_arr[i] = arr[i];
   }

   int selected = (int)SendMessage(hwndCombo, CB_GETCURSEL, 0, 0);

   switch (selected)
   {
   case 0:
      merge_sort(temp_arr, 0, n - 1);
      break;
   case 1:
      quicksort(temp_arr, 0, n - 1);
      break;
   case 2:
      bubble_sort(temp_arr, n);
      break;
   case 3:
      insertion_sort(temp_arr, n);
      break;
   case 4:
      selection_sort(temp_arr, n);
      break;
   }

   char output_buffer[2048] = "";
   for (int i = 0; i < n; i++)
   {
      char num_str[16];
      sprintf(num_str, "%d", temp_arr[i]);
      strcat(output_buffer, num_str);
      if (i < n - 1)
         strcat(output_buffer, " ");
   }

   SetWindowText(hwndOutput, output_buffer);
}