#include "SpreadSheetGateway.h"

#include <Arduino.h>
#include <StreamString.h>
#include <SPIFFS.h>

#include "AsyncHTTPClient.h"
#include "SecretConstants.h"
#include "RecordStorage.h"
#include "Constants.h"
#include "FileIOHelper.h"
#include "TimeProvider.h"

bool SpreadSheetGateway::PostRecords()
{
  AsyncHTTPClient http;
  http.begin(Constants::SpreadSheetScript::postUrl, Constants::SpreadSheetScript::rootCACertificate);

  http.setConnectTimeout(10 * 1000);
  http.setTimeout(6 * 60 * 1000);

  const auto count = RecordStorage::NumberOfRecords();

  // "0000/00/00 00:00:00,000.00,000.00,00000.00,0000,0000\n" で53バイト
  // https://wandbox.org/permlink/EtlMBKYJP0YKOap6
  const auto length = 53 * count;
  if (http.startAsyncRequest("POST", length) != 0) return false;

  // 実は File のデストラクタで close() が呼ばれるので、明示的に呼ぶ必要はない
  auto file = SPIFFS.open(Constants::Storage::Files::RECORDS, "r");

  for (int i = 0; i < count; i++) {
    const auto record = ReadFromFile<Record>(file);
    const auto& dateTime = record.dateTime;
    const auto& value = record.value;

    StreamString recordStr;
    recordStr.printf(
        "%s,%06.2f,%06.2f,%08.2f,%04d,%04d\n",
        TimeProvider::GetDateTimeStr(dateTime).c_str(),
        value.temperature,
        value.humidity,
        value.pressure,
        value.co2,
        value.tvoc
    );

    if (http.sendAsyncRequestPayload(recordStr) != 0) return false;
  }

  return http.finishAsyncRequest() == 200;
}
