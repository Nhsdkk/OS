#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <tuple>
#include <semaphore>

const size_t minRun = 15;
const int THREAD_SIZE = 4;

sem_t sem;

struct InsertParams {
    long* values;
    size_t start;
    size_t end;
};

struct MergeParams {
    long* values;
    size_t start1;
    size_t end1;
    size_t start2;
    size_t end2;
};

//enum SortedType {
//    Greater,
//    Less
//};

namespace timsort {

//    size_t getMinrun(size_t array_size) {
//        size_t f = 0;
//        while (array_size >= 64) {
//            f |= array_size & 1;
//            array_size >>= 1;
//        }
//        return array_size + f;
//    }

    void getSplits(std::queue<std::pair<size_t, size_t>>* splits, size_t size) {
        size_t curI = 0;
        while (curI < size){
            auto next = std::min(size, curI + minRun );
            splits->emplace(curI, next);
            curI = next;
        }
    }

    void move(std::vector<long>& values, long start, long end){
        while (start < end){
            values[end] = values[end - 1];
            end--;
        }
    }

    void insertionSort(long* values, size_t start, size_t end) {
        auto v = std::vector<long>(minRun);
        auto sz = 0;

        for (auto cur = start; cur < end; ++cur){
            auto i = 0;
            while (i < sz && values[cur] >= v[i]){
                i++;
            }

            if (i == sz){
                v[i] = values[cur];
                sz++;
                continue;
            }

            move(v, i, sz);
            sz++;
            v[i] = values[cur];
        }

        for (auto i = start; i < end; ++i){
            values[i] = v[i - start];
        }
    }

    void* insertSortThread(void* params){
        auto p = static_cast<InsertParams*>(params);
        insertionSort(p->values, p->start, p->end);
        std::free(params);
        sem_post(&sem);
        return nullptr;
    }

    void mergeSort(long* values, size_t start1, size_t end1, size_t start2, size_t end2){
        auto v = std::vector<long>(end2 - start1);

        auto i = start1;
        auto j = start2;
        auto k = 0;

        while (i < end1 && j < end2){
            if(values[i] < values[j]) {
                v[k] = values[i];
                ++i;
            }
            else {
                v[k] = values[j];
                ++j;
            }
            ++k;
        }

        while (i < end1){
            v[k] = values[i];
            ++i;
            ++k;
        }

        while (j < end2){
            v[k] = values[j];
            ++j;
            ++k;
        }

        for (auto s = start1; s < end2; ++s){
            values[s] = v[s - start1];
        }
    }

    void* mergeSortThread(void* params){
        auto p = static_cast<MergeParams*>(params);
        mergeSort(p->values, p->start1, p->end1, p->start2, p->end2);
        std::free(params);
        sem_post(&sem);
        return nullptr;
    }

    void sort(long *values, size_t size) {
        std::queue<std::pair<size_t, size_t>> splits;

        getSplits(&splits, size);

        while (!splits.empty()){
            auto p = splits.front();
            splits.pop();
            insertionSort(values, p.first, p.second);
        }

        auto mergeSize = minRun;

        while (mergeSize <= size){
            for (size_t start = 0; start < size; start += 2 * mergeSize) {
                size_t mid = std::min(start + mergeSize, size);
                size_t end = std::min(start + 2 * mergeSize, size);

                if (mid < end) {
                    mergeSort(values, start, mid, mid, end);
                }
            }

            mergeSize *= 2;
        }
    }

    void sortAsync(long *values, size_t size) {
        std::queue<std::pair<size_t, size_t>> splits;
        sem_init(&sem, 0, THREAD_SIZE);

        getSplits(&splits, size);

        pthread_t threads[splits.size()];

        auto threadCount = 0;
        int semVal;

        while (!splits.empty()){
            sem_wait(&sem);
            sem_getvalue(&sem, &semVal);
            std::cout << semVal << std::endl;
            auto p = splits.front();
            splits.pop();
            auto params = new InsertParams{values, p.first, p.second};
            pthread_create(&threads[threadCount], nullptr, insertSortThread, params);
            ++threadCount;
        }

        for (auto j = 0; j < threadCount; j++){
            pthread_join(threads[j], nullptr);
        }

        auto mergeSize = minRun;

        while (mergeSize <= size){
            threadCount = 0;
            for (size_t start = 0; start < size; start += 2 * mergeSize) {
                size_t mid = std::min(start + mergeSize, size);
                size_t end = std::min(start + 2 * mergeSize, size);

                if (mid < end) {
                    sem_wait(&sem);
                    auto params = new MergeParams{ values, start, mid, mid, end };
                    pthread_create(&threads[threadCount], nullptr, mergeSortThread, params);
                    ++threadCount;
                }
            }

            for (auto j = 0; j < threadCount; j++){
                pthread_join(threads[j], nullptr);
            }

            mergeSize *= 2;
        }

        sem_destroy(&sem);
    }
}