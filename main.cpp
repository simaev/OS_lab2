#include <windows.h>
#include <iostream>
#include <locale>

struct ArrayData {
    int* numbers;
    int    count;
    int    foundMin;
    int    foundMax;
    double foundAverage;
};

DWORD WINAPI FindMinMax(LPVOID param) {
    ArrayData* data = (ArrayData*)param;

    int currentMin = data->numbers[0];
    int currentMax = data->numbers[0];

    for (int i = 1; i < data->count; i++) {
        Sleep(7);
        if (data->numbers[i] < currentMin) currentMin = data->numbers[i];
        if (data->numbers[i] > currentMax) currentMax = data->numbers[i];
    }

    data->foundMin = currentMin;
    data->foundMax = currentMax;

    std::cout << "минимум: " << currentMin << "\n";
    std::cout << "максимум: " << currentMax << "\n";

    return 0;
}

typedef double (*CalculateAverageFunc)(int*, int);

DWORD WINAPI FindAverage(LPVOID param) {
    ArrayData* data = (ArrayData*)param;

    HMODULE libraryHandle = LoadLibraryA("MathLib.dll");
    CalculateAverageFunc calculateAverage =
        (CalculateAverageFunc)GetProcAddress(libraryHandle, "CalculateAverage");

    data->foundAverage = calculateAverage(data->numbers, data->count);

    FreeLibrary(libraryHandle);

    std::cout << "среднее: " << data->foundAverage << "\n";

    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");

    int size;
    std::cout << "введи размер массива: ";
    std::cin >> size;

    int* numbers = new int[size];
    std::cout << "введи элементы массива:\n";
    for (int i = 0; i < size; i++) {
        std::cout << "  [" << i << "]: ";
        std::cin >> numbers[i];
    }

    ArrayData data;
    data.numbers = numbers;
    data.count = size;

    HANDLE threadMinMax = CreateThread(NULL, 0, FindMinMax, &data, 0, NULL);
    HANDLE threadAverage = CreateThread(NULL, 0, FindAverage, &data, 0, NULL);

    WaitForSingleObject(threadMinMax, INFINITE);
    WaitForSingleObject(threadAverage, INFINITE);

    CloseHandle(threadMinMax);
    CloseHandle(threadAverage);

    for (int i = 0; i < size; i++) {
        if (numbers[i] == data.foundMin || numbers[i] == data.foundMax) {
            numbers[i] = (int)data.foundAverage;
        }
    }

    std::cout << "\nмассив после замены:\n";
    for (int i = 0; i < size; i++) {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";

    delete[] numbers;
    return 0;
}