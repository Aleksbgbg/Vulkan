#ifndef VULKAN_SRC_CORE_FILES_PNG_STREAM_H_
#define VULKAN_SRC_CORE_FILES_PNG_STREAM_H_

#include <array>

#include "core/types.h"

class BitStreamReader {
  using ReaderFunction = bool (BitStreamReader::*)();

 public:
  BitStreamReader(const u8* byteStream);

  bool ReadBit();
  u32 ReadBitsLsbFirst(u32 bitCount);
  u32 ReadBitsMsbFirst(u32 bitCount);

 private:
  bool ReadNextBit();
  bool ReadFinalBit();

 private:
  u8 byteValue_;
  const u8* byteStream_;
  u32 bitPosition_;
  std::array<ReaderFunction, 8> readerFunctions_;
};

class ByteStreamReader {
 public:
  struct MarkedPosition {
   private:
    friend ByteStreamReader;
    MarkedPosition(const u8* const byte) : byte(byte) {}
    const u8* const byte;
  };

  ByteStreamReader(const u8* data);

  const u8* ReadBytes(u32 size);

  u8 ReadByte();

  u32 ReadInt();

  MarkedPosition MarkPosition(u32 offset) const;

  void Jump(MarkedPosition position);

  BitStreamReader AsBitStream() const;

 private:
  const u8* byte_;
};

class ByteStreamReadWriter {
 public:
  ByteStreamReadWriter(u8* data);

  u32 WrittenSize() const;
  u32 ReadByte(u32 index) const;

  void WriteByte(u8 value);

 private:
  u8* data_;
  u32 offset_;
};

#endif  // VULKAN_SRC_CORE_FILES_PNG_STREAM_H_
