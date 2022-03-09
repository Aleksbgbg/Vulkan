#include "stream.h"

#include "core/math/bytes.h"
#include "core/types.h"

BitStreamReader::BitStreamReader(const u8* const byteStream)
    : byteValue_(*byteStream), byteStream_(byteStream + 1), bitPosition_(0) {
  for (u32 index = 0; index < 7; ++index) {
    readerFunctions_[index] = &BitStreamReader::ReadNextBit;
  }
  readerFunctions_[7] = &BitStreamReader::ReadFinalBit;
}

bool BitStreamReader::ReadBit() {
  return (this->*(readerFunctions_[bitPosition_]))();
}

u32 BitStreamReader::ReadBitsLsbFirst(const u32 bitCount) {
  u32 value = 0;

  for (u32 bitIndex = 0; bitIndex < bitCount; ++bitIndex) {
    value |= static_cast<u32>(ReadBit()) << bitIndex;
  }

  return value;
}

u32 BitStreamReader::ReadBitsMsbFirst(const u32 bitCount) {
  u32 value = 0;

  for (u32 bitIndex = 0; bitIndex < bitCount; ++bitIndex) {
    value <<= 1;
    value |= static_cast<u32>(ReadBit());
  }

  return value;
}

bool BitStreamReader::ReadNextBit() {
  return (byteValue_ >> bitPosition_++) & 0b1;
}

bool BitStreamReader::ReadFinalBit() {
  const bool bit = ReadNextBit();
  byteValue_ = *byteStream_++;
  bitPosition_ = 0;
  return bit;
}

ByteStreamReader::ByteStreamReader(const u8* const data) : byte_(data) {}

const u8* ByteStreamReader::ReadBytes(const u32 size) {
  const u8* const bytesStart = byte_;
  byte_ += size;
  return bytesStart;
}

u8 ByteStreamReader::ReadByte() {
  return *ReadBytes(1);
}

u32 ByteStreamReader::ReadInt() {
  const u8* const bytes = ReadBytes(4);
  return BytesToInt(bytes);
}

ByteStreamReader::MarkedPosition ByteStreamReader::MarkPosition(
    const u32 offset) const {
  return MarkedPosition(byte_ + offset);
}

void ByteStreamReader::Jump(const ByteStreamReader::MarkedPosition position) {
  byte_ = position.byte;
}

BitStreamReader ByteStreamReader::AsBitStream() const {
  return BitStreamReader(byte_);
}

ByteStreamReadWriter::ByteStreamReadWriter(u8* const data)
    : data_(data), offset_(0) {}

u32 ByteStreamReadWriter::WrittenSize() const {
  return offset_;
}

u32 ByteStreamReadWriter::ReadByte(const u32 index) const {
  return data_[index];
}

void ByteStreamReadWriter::WriteByte(const u8 value) {
  data_[offset_] = value;
  ++offset_;
}
