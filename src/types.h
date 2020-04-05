#pragma once

#include "errors.h"
#include <cstdint>
#include <istream>
#include <string>
#include <vector>

#pragma pack(push)
#pragma pack(1)

class ErrorCodeException : public std::exception {
public:
  ErrorCodeException(ErrorCode code) : m_Code(code) { }
  virtual const char *what() const throw() { return "An error occurred"; }
  ErrorCode code() const throw() { return m_Code; }
private:
  ErrorCode m_Code;
};


struct Item {
  uint32_t offset;
  uint32_t count;
};

static const char HEADER_SIGNATURE[8] = "CryXmlB";

struct Header;

struct Node {
  uint32_t tagOffset;
  uint32_t contentOffset;
  uint16_t numAttributes;
  uint16_t numChildren;
  uint32_t parentIdx;
  uint32_t attributeOffset;
  uint32_t childrenOffset;
  uint32_t unknown;

  static Node readFrom(const std::vector<uint8_t> &input, const Header &hdr, int nodeIndex);
};

struct Attribute {
  uint32_t key;
  uint32_t value;

  static Attribute readFrom(const std::vector<uint8_t> &input, const Header &hdr, int nodeIndex);
};

struct Header {
  char signature[8];
  uint32_t fileSize;

  Item nodeInfo;
  Item attributes;
  Item nodeHierarchy;
  Item stringList;

  uint8_t unknown[24];
};

#pragma pack(pop)

class CryXml {
public:
  CryXml(std::istream &input);
  CryXml(const char *buffer, size_t bufferSize);

  Node getNode(uint32_t offset);

  uint32_t getNodeRef(uint32_t offset);

  Attribute getAttribute(uint32_t idx);

  std::string getString(uint32_t offset);

private:
  std::vector<uint8_t> m_Data;
  // std::istream &m_Data;
  Header m_Header;

};

