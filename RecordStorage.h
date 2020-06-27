#ifndef RECORD_STORAGE_H
#define RECORD_STORAGE_H

#include "Record.h"

class RecordStorage {
public:
  static void Begin();

  static void End();

  static int NumberOfRecords();

  static Record ReadLatestRecord();

  static void SaveRecord(const Record record);

  static void RemoveAllRecords();

  static DateTime GetLatestRecordTime();
};

#endif
