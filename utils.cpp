//
// Created by Pavel Mukhanov on 27/12/2018.
//

#include <iostream>
#include <rocksdb/status.h>
#include "utils.h"


void checkStatus(const rocksdb::Status &status) {
    if (!status.ok()) {
        std::cerr << "status: " << status.ToString() << "\n";
    }
    assert(status.ok());
}
