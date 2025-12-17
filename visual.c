#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "sortlib.h"

#define ID_COMBO_SORT 100
#define ID_INPUT 101
#define ID_INPUT_VAL 102
#define ID_GENERATE 103
#define ID_SORT 104
#define ID_OUTPUT 105
#define ID_TIME 106
#define ID_STATUS 107

HINSTANCE hInst;
HWND hwndCombo, hwndInput, hwndInputVal, hwndOutput, hwndTime, hwndStatus;
HBRUSH g_hBrush = NULL;
HBRUSH g_editBrush = NULL;
HBRUSH g_readonlyBrush = NULL;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GenerateRandomList(HWND);
void SortList(HWND);
int ValidateInput(const char* input);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand(time(NULL));
    const char* szClassName = "MainWindow";
    hInst = hInstance;

    g_hBrush = CreateSolidBrush(RGB(40, 44, 52));
    g_readonlyBrush = CreateSolidBrush(RGB(50, 55, 65));

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
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_CREATE: {
            hwndCombo = CreateWindow(
                "COMBOBOX", "",  
                CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
                10, 10, 200, 200,
                hwnd, (HMENU)ID_COMBO_SORT, hInst, NULL
            );
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)"Merge Sort");
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)"Quick Sort");
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)"Bubble Sort");
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)"Insertion Sort");
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)"Selection Sort");
            SendMessage(hwndCombo, CB_SETCURSEL, 0, 0); 

            hwndInput = CreateWindow(
                "EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
                220, 10, 550, 100,
                hwnd, (HMENU)ID_INPUT, hInst, NULL
            );
            
            hwndInputVal = CreateWindow(
                "EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                330, 120, 100, 25,
                hwnd, (HMENU)ID_INPUT_VAL, hInst, NULL
            );
            
            CreateWindow(
                "STATIC", "Count:",
                WS_CHILD | WS_VISIBLE,
                270, 120, 60, 25,
                hwnd, NULL, hInst, NULL
            );

            CreateWindow(
                "BUTTON", "Generate",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 40, 80, 30,
                hwnd, (HMENU)ID_GENERATE, hInst, NULL
            );

            CreateWindow(
                "BUTTON", "Sort",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                100, 40, 80, 30,
                hwnd, (HMENU)ID_SORT, hInst, NULL
            );

            hwndOutput = CreateWindow(
                "EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                10, 160, 760, 350,
                hwnd, (HMENU)ID_OUTPUT, hInst, NULL
            );
            
            CreateWindow(
                "STATIC", "Execution Time (ms):",
                WS_CHILD | WS_VISIBLE,
                10, 520, 120, 20,
                hwnd, NULL, hInst, NULL
            );
            
            hwndTime = CreateWindow(
                "EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY,
                130, 520, 100, 20,
                hwnd, (HMENU)ID_TIME, hInst, NULL
            );
            
            CreateWindow(
                "STATIC", "Status:",
                WS_CHILD | WS_VISIBLE,
                240, 520, 60, 20,
                hwnd, NULL, hInst, NULL
            );
            
            hwndStatus = CreateWindow(
                "EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY,
                300, 520, 470, 20,
                hwnd, (HMENU)ID_STATUS, hInst, NULL
            );
            break;
        }
        
        case WM_CTLCOLORDLG:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            HWND hwndCtl = (HWND)lParam;

            if (hwndCtl == hwndInput || hwndCtl == hwndOutput) {
                SetTextColor(hdc, RGB(220, 220, 220));
                SetBkColor(hdc, RGB(60, 65, 75));
                if (!g_editBrush) g_editBrush = CreateSolidBrush(RGB(60, 65, 75));
                return (INT_PTR)g_editBrush;
            } else if (hwndCtl == hwndTime || hwndCtl == hwndStatus || hwndCtl == hwndInputVal) {
                SetTextColor(hdc, RGB(220, 220, 220));
                SetBkColor(hdc, RGB(50, 55, 65));
                if (!g_readonlyBrush) g_readonlyBrush = CreateSolidBrush(RGB(50, 55, 65));
                return (INT_PTR)g_readonlyBrush;
            }
            break;
        }
        
        case WM_COMMAND: {
            if(LOWORD(wParam) == ID_GENERATE) {
                GenerateRandomList(hwnd);
            } else if(LOWORD(wParam) == ID_SORT) {
                SortList(hwnd);
            }
            break;
        }
        
        case WM_DESTROY:
            if (g_hBrush) DeleteObject(g_hBrush);
            if (g_editBrush) DeleteObject(g_editBrush);
            if (g_readonlyBrush) DeleteObject(g_readonlyBrush);
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void GenerateRandomList(HWND hwnd) {
    char input_buffer[1024];
    GetWindowText(hwndInputVal, input_buffer, sizeof(input_buffer));
    
    long long count = 1;
    if (input_buffer[0] != '\0') {
        count = atoll(input_buffer);
        if (count <= 0) count = 1;
        if (count > 1000000) count = 1000000; 
    } else {
        count = 10 + rand() % 10; 
    }
    
    int *numbers = malloc(count * sizeof(int));

    for(long long i = 0; i < count; i++) {
        numbers[i] = rand() % 1000;
    }
    char *buffer = malloc((count * 8 + count - 1 + 1) * sizeof(char));
    char *ptr = buffer;
    
    for(long long i = 0; i < count; i++) {
        ptr += sprintf(ptr, "%d", numbers[i]);
        if(i < count - 1) {
            *ptr = ' ';
            ptr++;
        }
    }
    *ptr = '\0';
    
    SetWindowText(hwndInput, buffer);
    
    char status_str[64];
    sprintf(status_str, "Generated %lld numbers", count);
    SetWindowText(hwndStatus, status_str);
    
    free(numbers);
    free(buffer);
}

int ValidateInput(const char* input) {
    if (!input || !*input) return 0;
    
    int in_number = 0;
    int has_space = 0;
    const char* p = input;
    
    while (*p) {
        if (isspace(*p)) {
            if (in_number) {
                in_number = 0;
            }
        } else if (isdigit(*p) || (*p == '-' && isdigit(*(p+1)) && (!p || isspace(*(p-1)) || p == input))) {
            in_number = 1;
        } else {
            return 0;
        }
        p++;
    }
    
    return in_number;
}

void SortList(HWND hwnd) {
    char input_buffer[1024 * 100]; 
    GetWindowText(hwndInput, input_buffer, sizeof(input_buffer));
    
    if (!ValidateInput(input_buffer)) {
        MessageBox(hwnd, "Invalid input! ", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    long long n = 0;
    char *temp_input = malloc(strlen(input_buffer) + 1);
    strcpy(temp_input, input_buffer);
    
    char *token = strtok(temp_input, " ");
    while(token != NULL) {
        n++;
        token = strtok(NULL, " ");
    }
    
    if (n == 0) {
        MessageBox(hwnd, "No valid numbers found!", "Error", MB_OK | MB_ICONERROR);
        free(temp_input);
        return;
    }

    if (n > 1000000) {
        MessageBox(hwnd, "More than 1 000 000", "Error", MB_OK | MB_ICONERROR);
        free(temp_input);
        return;
    }

    int *arr = malloc(n * sizeof(int));
    int *temp_arr = malloc(n * sizeof(int));
    
    strcpy(temp_input, input_buffer);
    token = strtok(temp_input, " ");
    long long idx = 0;
    while(token != NULL && idx < n) {
        arr[idx] = atoi(token);
        temp_arr[idx] = arr[idx];
        idx++;
        token = strtok(NULL, " ");
    }
    
    SetWindowText(hwndStatus, "Sorting...");
    UpdateWindow(hwndStatus);
    
    clock_t start_time = clock();
    
    int selected = (int)SendMessage(hwndCombo, CB_GETCURSEL, 0, 0);
    
    switch(selected) {
        case 0: 
            merge_sort_count(temp_arr, 0, n - 1);
            break;
        case 1: 
            quicksort_count(temp_arr,  n );
            break;
        case 2:
            bubble_sort_count(temp_arr, n);
            break;
        case 3: 
            insertion_sort_count(temp_arr, n);
            break;
        case 4: 
            selection_sort_count(temp_arr, n);
            break;
    }
    
    clock_t end_time = clock();
    double execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0; // в миллисекундах
    
    char *output_buffer = malloc((n * 8 + n - 1 + 1) * sizeof(char));
    char *out_ptr = output_buffer;
    
    for(long long i = 0; i < n; i++) {
        out_ptr += sprintf(out_ptr, "%d", temp_arr[i]);
        if(i < n - 1) {
            *out_ptr = ' ';
            out_ptr++;
        }
    }
    *out_ptr = '\0';
    
    SetWindowText(hwndOutput, output_buffer);
    char time_str[32];
    sprintf(time_str, "%.2f", execution_time);
    SetWindowText(hwndTime, time_str);
    
    char status_str[128];
    sprintf(status_str, "Sorted %lld numbers in %.2f ms", n, execution_time);
    SetWindowText(hwndStatus, status_str);
    
    free(arr);
    free(temp_arr);
    free(temp_input);
    free(output_buffer);
}