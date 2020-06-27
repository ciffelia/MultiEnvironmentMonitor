#include "RecordStorage.h"

#include <SPIFFS.h>

#include "Constants.h"
#include "FileIOHelper.h"

using namespace Constants::Storage;

void RecordStorage::Begin()
{
  SPIFFS.begin();
}

void RecordStorage::End()
{
  SPIFFS.end();
}

int RecordStorage::NumberOfRecords()
{
  const auto file = SPIFFS.open(Files::RECORDS, "r");
  return file.size() / sizeof(Record);
}

Record RecordStorage::ReadLatestRecord()
{
  auto file = SPIFFS.open(Files::RECORDS, "r");

  file.seek(-sizeof(Record), SeekEnd);

  return ReadFromFile<Record>(file);
}

void RecordStorage::SaveRecord(const Record record)
{
  auto recordsFile = SPIFFS.open(Files::RECORDS, "a");
  WriteToFile(recordsFile, record);

  auto latestRecordTimeFile = SPIFFS.open(Files::LATEST_RECORD_TIME, "w");
  WriteToFile(latestRecordTimeFile, record.dateTime);
}

void RecordStorage::RemoveAllRecords()
{
  SPIFFS.remove(Files::RECORDS);
}

DateTime RecordStorage::GetLatestRecordTime()
{
  auto file = SPIFFS.open(Files::LATEST_RECORD_TIME, "r");

  return ReadFromFile<DateTime>(file);
}
