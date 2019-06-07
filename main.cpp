#include <iostream>
#include <cassert>
#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
#include "column_family.h"

using namespace rocksdb;

std::string kDBPath = "/tmp/testdb";

int main() {

	 reuse_transaction(kDBPath);

}
