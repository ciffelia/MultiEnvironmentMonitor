#ifndef ASYNC_HTTP_CLIENT_H
#define ASYNC_HTTP_CLIENT_H

#include <HTTPClient.h>

class AsyncHTTPClient : public HTTPClient {
public:
  int startAsyncRequest(const char *type, const size_t length);

  int sendAsyncRequestPayload(const char* payload, const size_t size);

  int sendAsyncRequestPayload(const String payload);

  int finishAsyncRequest();
};

#endif
