#pragma once

#include "dll.h"

extern "C" {
  DLLEXPORT int cryxml_decode_file(const char *encodedPath, const char *outputPath);
  DLLEXPORT int cryxml_decode_buffer(const char *buffer, size_t bufferSize, char **output);
  DLLEXPORT void cryxml_free(char *buffer);

  DLLEXPORT const char *cryxml_error_to_string(int code);
}

