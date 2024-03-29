#include "libcryxml.h"
#include "types.h"
#include "errors.h"
#include <fstream>
#include <string>
#include <tinyxml2.h>

tinyxml2::XMLElement *convertNode(CryXml &input, tinyxml2::XMLDocument &doc, int offset) {
  Node inputNode = input.getNode(offset);

  tinyxml2::XMLElement *result = doc.NewElement(input.getString(inputNode.tagOffset).c_str());

  for (uint32_t i = 0; i < inputNode.numAttributes; ++i) {
    Attribute attr = input.getAttribute(inputNode.attributeOffset + i);
    result->SetAttribute(input.getString(attr.key).c_str(), input.getString(attr.value).c_str());
  }

  for (uint32_t i = 0; i < inputNode.numChildren; ++i) {
    uint32_t nodeOffset = input.getNodeRef(inputNode.childrenOffset + i);
    result->InsertEndChild(convertNode(input, doc, nodeOffset));
  }

  return result;
}

DLLEXPORT int cryxml_decode_file(const char *encodedPath, const char *outputPath) {
  std::ifstream input(encodedPath, std::ios::binary | std::ios::in);

  try {
    CryXml inputXml(input);

    tinyxml2::XMLDocument doc;
    doc.InsertFirstChild(convertNode(inputXml, doc, 0));

    if (doc.SaveFile(outputPath) != tinyxml2::XML_SUCCESS) {
      return ERROR_FAILED_TO_GENERATE_XML;
    }

    return ERROR_NONE;
  }
  catch (const ErrorCodeException &e) {
    return e.code();
  }
  catch (...) {
    return ERROR_UNKNOWN;
  }
}

DLLEXPORT int cryxml_decode_buffer(const char *buffer, size_t bufferSize, char **output) {
  *output = nullptr;
  try {
    CryXml inputXml(buffer, bufferSize);

    tinyxml2::XMLDocument doc;
    doc.InsertFirstChild(convertNode(inputXml, doc, 0));

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);

    *output = new char[printer.CStrSize()];
    memcpy(*output, printer.CStr(), printer.CStrSize());

    return ERROR_NONE;
  }
  catch (const ErrorCodeException &e) {
    return e.code();
  }
  catch (...) {
    return ERROR_UNKNOWN;
  }
}

DLLEXPORT void cryxml_free(char *buffer) {
  delete[] buffer;
}

DLLEXPORT const char *cryxml_error_to_string(int code) {
  switch (code) {
  case ERROR_NONE: return "No error";
  case ERROR_FAILED_TO_GENERATE_XML: return "Failed to generate xml";
  case ERROR_INVALID_SIGNATURE: return "Invalid signature";
  default: return "Unknown error";
  }
}

