#include "AsyncHTTPClient.h"

int AsyncHTTPClient::startAsyncRequest(const char *type, const size_t length)
{
  if(!connect())
    return returnError(HTTPC_ERROR_CONNECTION_REFUSED);

  addHeader(F("Content-Length"), String(length));

  if(!sendHeader(type))
    return returnError(HTTPC_ERROR_SEND_HEADER_FAILED);

  return 0;
}

int AsyncHTTPClient::sendAsyncRequestPayload(const char* payload, const size_t size)
{
  if (_client->write((const uint8_t*) payload, size) != size)
    return returnError(HTTPC_ERROR_SEND_PAYLOAD_FAILED);

  return 0;
}

int AsyncHTTPClient::sendAsyncRequestPayload(const String payload)
{
  return sendAsyncRequestPayload(payload.c_str(), payload.length());
}

int AsyncHTTPClient::finishAsyncRequest()
{
  return returnError(handleHeaderResponse());
}
