//
// Created by nikit on 12/4/2024.
//

#include "f1.h"

float CalcPiLeibniz(int k) {
    float res = 0.0f;
    float one = 1.0f;

    for (int i = 0; i < k; i++) {
        res += one / (2.0f * i + 1.0f);
        one *= -1;
    }

    return res * 4.0f;
}

std::vector<int> SortBubble(std::vector<int> array) {
    auto swp = true;

    while (swp) {
        swp = false;
        for (auto j = 0; j < array.size() - 1; ++j) {
            auto curVal1 = array[j];
            auto curVal2 = array[j + 1];
            if (curVal2 < curVal1) {
                swp = true;
                array[j] = curVal2;
                array[j + 1] = curVal1;
            }
        }
    }

    return array;
}
