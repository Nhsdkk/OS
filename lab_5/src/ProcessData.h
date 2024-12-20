//
// Created by Anton on 12/9/2024.
//

#ifndef OS_LAB_5_SRC_PROCESSDATA_H_
#define OS_LAB_5_SRC_PROCESSDATA_H_

namespace Process {

    class ProcessData {
        long id;
        public:
            explicit ProcessData(long id): id(id) {}
            bool operator==(const ProcessData& other) const { return id == other.id; }
            bool operator<(const ProcessData& other) const { return id < other.id; }
            bool operator>(const ProcessData& other) const { return id > other.id; }
                long getId() const { return id; }
            ~ProcessData() = default;
    };

} // Process

#endif //OS_LAB_5_SRC_PROCESSDATA_H_
