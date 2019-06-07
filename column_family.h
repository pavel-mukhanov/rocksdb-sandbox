//
// Created by Pavel Mukhanov on 27/12/2018.
//

#ifndef ROCKSDB_SANDBOX_COLUMN_FAMILY_H
#define ROCKSDB_SANDBOX_COLUMN_FAMILY_H

#include <string>

void create_cf(std::string kDBPath);
void reuse_transaction(std::string kDBPath);

#endif //ROCKSDB_SANDBOX_COLUMN_FAMILY_H
