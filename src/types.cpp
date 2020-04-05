#include "types.h"
#include <stdexcept>

Node Node::readFrom(const std::vector<uint8_t> &input, const Header & hdr, int nodeOffset) {
  uint32_t offset = hdr.nodeInfo.offset + nodeOffset * sizeof(Node);

  Node result;
  memcpy(&result, input.data() + offset, sizeof(Node));

  return result;
}

Attribute Attribute::readFrom(const std::vector<uint8_t> &input, const Header &hdr, int index) {
  uint32_t offset = hdr.attributes.offset + index * sizeof(Attribute);

  Attribute result;
  memcpy(&result, input.data() + offset, sizeof(Attribute));

  return result;
}

CryXml::CryXml(std::istream &input)
{
  input.seekg(0, std::ios::end);
  size_t fileSize = input.tellg();

  if (fileSize < sizeof(Header)) {
    throw ErrorCodeException(ERROR_INVALID_SIGNATURE);
  }

  // try to read just the header first, to avoid reading the entire file just to find it's not
  // a cryxml file
  input.seekg(0);
  input.read(reinterpret_cast<char*>(&m_Header), sizeof(Header));
  if (memcmp(m_Header.signature, HEADER_SIGNATURE, 8) != 0) {
    throw ErrorCodeException(ERROR_INVALID_SIGNATURE);
  }

  input.seekg(0);
  m_Data.resize(fileSize);
  input.read(reinterpret_cast<char*>(&m_Data[0]), fileSize);
}

CryXml::CryXml(const char *buffer, size_t bufferSize) {
  if (bufferSize < sizeof(Header)) {
    throw ErrorCodeException(ERROR_INVALID_SIGNATURE);
  }

  memcpy(&m_Header, buffer, sizeof(Header));
  if (memcmp(m_Header.signature, HEADER_SIGNATURE, 8) != 0) {
    throw ErrorCodeException(ERROR_INVALID_SIGNATURE);
  }

  m_Data.resize(bufferSize);
  memcpy(&m_Data[0], buffer, bufferSize);
}

Node CryXml::getNode(uint32_t offset) {
  return Node::readFrom(m_Data, m_Header, offset);
}

uint32_t CryXml::getNodeRef(uint32_t offset) {
  uint32_t result;

  uint32_t bufferOffset = m_Header.nodeHierarchy.offset + offset * 4;
  memcpy(reinterpret_cast<char*>(&result), m_Data.data() + bufferOffset, sizeof(uint32_t));
  return result;
}

Attribute CryXml::getAttribute(uint32_t idx) {
  return Attribute::readFrom(m_Data, m_Header, idx);
}

std::string CryXml::getString(uint32_t offset) {
  uint32_t bufferOffset = m_Header.stringList.offset + offset;

  size_t length = strcspn(reinterpret_cast<char*>(m_Data.data()) + bufferOffset, "\0");

  std::string result;
  result.resize(length);
  memcpy(&result[0], m_Data.data() + bufferOffset, length);

  return result;
}
