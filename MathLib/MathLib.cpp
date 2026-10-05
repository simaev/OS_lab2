#include "pch.h"
#include "MathLib.h"

extern "C" __declspec(dllexport) double CalculateAverage(int* numbers, int count) {
    double total = 0;
    for (int i = 0; i < count; i++) {
        Sleep(12);
        total += numbers[i];
    }
    return total / count;
}