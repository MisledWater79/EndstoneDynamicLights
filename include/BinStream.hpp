#pragma once

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <sstream>
#include <locale>

typedef uint32_t uint24_t;
typedef int32_t int24_t;

class BinStream {
	uint8_t* data;
	uint32_t size;
	uint32_t position;

public:

	BinStream();
	BinStream(uint8_t* data, uint32_t length);
	BinStream(uint32_t length);
	BinStream(const char* data, uint32_t length);

	~BinStream();

	uint8_t* getData();
	uint32_t getSize();
	uint32_t getPosition();

	void setData(uint8_t* data, uint32_t length);
	void setSize(uint32_t length);
	void setPosition(uint32_t position);

	template <typename T>
	void write(T* value, bool endian = true) {
		value->write(this, endian);
	};
	void writeUVarInt(uint32_t value, bool endian = true);
	void writeUVarLong(uint64_t value, bool endian = true);
	void writeUint8(uint8_t value, bool endian = true);
	void writeUint16(uint16_t value, bool endian = true);
	void writeUint24(uint24_t value, bool endian = true);
	void writeUint32(uint32_t value, bool endian = true);
	void writeUint64(uint64_t value, bool endian = true);

	void writeVarInt(int32_t value, bool endian = true);
	void writeVarLong(int64_t value, bool endian = true);
	void writeInt8(int8_t value, bool endian = true);
	void writeInt16(int16_t value, bool endian = true);
	void writeInt24(int24_t value, bool endian = true);
	void writeInt32(int32_t value, bool endian = true);
	void writeInt64(int64_t value, bool endian = true);
	void writeFloat(float value, bool endian = true);
	void writeDouble(double value, bool endian = true);

	template <typename T>
	T* read(T value, bool endian = true) {
		T* v = new T();
		v->read(this, endian);
		return v;
	};
	uint32_t readUVarInt(bool endian = true);
	int32_t readVarInt(bool endian = true);
	uint64_t readUVarLong(bool endian = true);
	int64_t readVarLong(bool endian = true);
	uint8_t readUint8(bool endian = true);
	uint16_t readUint16(bool endian = true);
	uint24_t readUint24(bool endian = true);
	uint32_t readUint32(bool endian = true);
	uint64_t readUint64(bool endian = true);
	int8_t readInt8(bool endian = true);
	int16_t readInt16(bool endian = true);
	int24_t readInt24(bool endian = true);
	int32_t readInt32(bool endian = true);
	int64_t readInt64(bool endian = true);
	float readFloat(bool endian = true);
	double readDouble(bool endian = true);

	void writeString(std::string value, bool endian = true);
	void writeString16(std::string value, bool endian = true);
	std::string readString(bool endian = true);

	void writeBytes(const uint8_t* value, uint32_t length, bool endian = true);
	uint8_t* readBytes(uint32_t length, bool endian = true);

	void writeBinStream(BinStream value, bool endian = true);
	BinStream readBinStream(bool endian = true);

	void reset();
	void clear();

	std::string print();
};