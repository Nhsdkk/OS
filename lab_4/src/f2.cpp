//
// Created by nikit on 12/4/2024.
//

#include "f2.h"

float CalcPiVallis(int k) {
    float result = 1;

    for (auto n = 1; n <= k; ++n){
        result *= (4.0f * n * n) / (4.0f * n * n - 1.0f);
    }

    return 2 * result;
}

std::vector<int> SortHoar(std::vector<int> array){
    auto size = array.size();
    if (size < 2) return array;

    auto idxCenter = size / 2;
    auto mid = array[idxCenter];

    std::vector<int> temp1;
    std::vector<int> temp2;

    for (int i = 0; i < size; ++i){
        if (i == idxCenter) continue;
        if (array[i] < mid) temp1.push_back(array[i]);
        else temp2.push_back(array[i]);
    }

    temp1 = SortHoar(temp1);
    temp2 = SortHoar(temp2);

    std::vector<int> result;

    for (auto& item: temp1) result.push_back(item);
    result.push_back(mid);
    for (auto& item: temp2) result.push_back(item);

    return result;

}