//
// Created by Pavel Mukhanov on 27/12/2018.
//

#include "column_family.h"

#include <iostream>
#include <cassert>
#include "rocksdb/db.h"
#include "rocksdb/utilities/transaction_db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"
#include "utils.h"

using namespace rocksdb;

void create_cf(std::string kDBPath) {
    // open DB with two column families
    std::vector<ColumnFamilyDescriptor> column_families;
    // have to open default column family
    column_families.emplace_back(
            kDefaultColumnFamilyName, ColumnFamilyOptions());
    // open the new one, too
    column_families.emplace_back(
            "new_cf", ColumnFamilyOptions());
    std::vector<ColumnFamilyHandle*> handles;

    DB* db;
    Options options;
    options.create_if_missing = true;
    Status status = DB::Open(options, kDBPath, column_families, &handles, &db);
    checkStatus(status);

    std::string value;

    Slice key1 = "single_key";
    status = db->Put(rocksdb::WriteOptions(), key1, "value");
    status = db->Get(rocksdb::ReadOptions(), key1, &value);

    std::cout << value << "\n";

    // create column family
    ColumnFamilyHandle* cf;

    // list column families
    std::vector<std::string> columnFamilies;
    status = DB::ListColumnFamilies(options, "/tmp/testdb", &columnFamilies);
    checkStatus(status);

    std::cout << "available cf's: " << "\n";
    for (const auto &columnFamily : columnFamilies) {
        std::cout << "\t" << columnFamily << "\n";
    }

    if (std::find(columnFamilies.begin(), columnFamilies.end(), "new_cf") == columnFamilies.end()) {
        status = db->CreateColumnFamily(ColumnFamilyOptions(), "new_cf", &cf);
        checkStatus(status);
        delete cf;
    }

    // close DB
    delete db;

    status = DB::Open(DBOptions(), kDBPath, column_families, &handles, &db);
    assert(status.ok());

    // put and get from non-default column family
    status = db->Put(WriteOptions(), handles[1], Slice("key"), Slice("value"));
    assert(status.ok());
    status = db->Get(ReadOptions(), handles[1], Slice("key"), &value);
    assert(status.ok());

    // atomic write
    WriteBatch batch;
    batch.Put(handles[0], Slice("key2"), Slice("value2"));
    batch.Put(handles[1], Slice("key3"), Slice("value3"));
    batch.Delete(handles[0], Slice("key"));
    status = db->Write(WriteOptions(), &batch);
    assert(status.ok());

    for (auto handle : handles) {
        delete handle;
    }
    delete db;
}

void reuse_transaction(std::string kDBPath) {
	  // open DB
  Options options;
  TransactionDBOptions txn_db_options;
  options.create_if_missing = true;
  TransactionDB* txn_db;

  Status s = TransactionDB::Open(options, txn_db_options, kDBPath, &txn_db);
  assert(s.ok());

  WriteOptions write_options;
  ReadOptions read_options;
  TransactionOptions txn_options;
  std::string value;

   // Start a transaction
  Transaction* txn = txn_db->BeginTransaction(write_options);
  assert(txn);

  // Read a key in this transaction
  s = txn->Get(read_options, "abc", &value);

  // Write a key in this transaction
  s = txn->Put("abc", "foobar");
  assert(s.ok());

  // Read a key OUTSIDE this transaction. Does not affect txn.
  s = txn_db->Get(read_options, "abc", &value);

  // Write a key OUTSIDE of this transaction.
  // Does not affect txn since this is an unrelated key.  If we wrote key 'abc'
  // here, the transaction would fail to commit.
  s = txn_db->Put(write_options, "xyz", "zzz");

  // Commit transaction
  //s = txn->Commit();
  //assert(s.ok());
  delete txn;

  // Set a snapshot at start of transaction by setting set_snapshot=true
  //txn_options.set_snapshot = true;
  txn = txn_db->BeginTransaction(write_options, txn_options);

  s = txn_db->Get(read_options, "abc", &value);
  std::cout << value << "\n";

  delete txn_db;

}

