#include "BinStream.hpp"


BinStream::BinStream() {
	this->data = new uint8_t[0];
	this->size = 0;
	this->position = 0;
}

BinStream::BinStream(uint8_t* data, uint32_t length) {
	this->data = new uint8_t[length];
	memcpy(this->data, data, length);
	this->size = length;
	this->position = 0;
}

BinStream::BinStream(uint32_t length) {
	this->data = new uint8_t[length];
	this->size = length;
	this->position = 0;
}
BinStream::BinStream(const char* data, uint32_t length) {
	this->data = new uint8_t[length];
	memcpy(this->data, data, length);
	this->size = length;
	this->position = 0;
}

BinStream::~BinStream() {
	delete[] this->data;
}

uint8_t* BinStream::getData() {
	return this->data;
}

uint32_t BinStream::getSize() {
	return this->size;
}

uint32_t BinStream::getPosition() {
	return this->position;
}

void BinStream::setData(uint8_t* data, uint32_t length) {
	this->data = data;
	this->size = length;
	this->position = 0;
}

void BinStream::setSize(uint32_t length) {
	this->size = length;
}

void BinStream::setPosition(uint32_t position) {
	this->position = position;
}

uint32_t BinStream::readUVarInt(bool endian) {
	uint32_t value = 0;
	int shift = 0;
	while (true) {
		if (this->position >= this->size) return 0; // Out of bounds
		uint8_t byte = this->data[this->position++];
		value |= (uint32_t)(byte & 0x7F) << shift;
		if ((byte & 0x80) == 0) break;
		shift += 7;
		if (shift > 35) return 0; // Prevent overflow
	}
	return value;
}

// Reads a signed VarInt (with ZigZag decoding)
int32_t BinStream::readVarInt(bool endian) {
	uint32_t raw = readUVarInt(endian);
	return (int32_t)((raw >> 1) ^ (~(raw & 1) + 1));
}

// Reads an unsigned VarLong (no ZigZag)
uint64_t BinStream::readUVarLong(bool endian) {
	uint64_t value = 0;
	int shift = 0;
	while (true) {
		if (this->position >= this->size) return 0; // Out of bounds
		uint8_t byte = this->data[this->position++];
		value |= (uint64_t)(byte & 0x7F) << shift;
		if ((byte & 0x80) == 0) break;
		shift += 7;
		if (shift > 70) return 0; // Prevent overflow
	}
	return value;
}

// Reads a signed VarLong (with ZigZag decoding)
int64_t BinStream::readVarLong(bool endian) {
	uint64_t raw = readUVarLong(endian);
	return (int64_t)((raw >> 1) ^ (~(raw & 1) + 1));
}

void BinStream::writeUVarInt(uint32_t value, bool endian) {
	while (true) {
		uint8_t byte = value & 0x7F;
		value >>= 7;
		if (value != 0) byte |= 0x80;
		this->writeUint8(byte);
		if (value == 0) break;
	}
}

void BinStream::writeUVarLong(uint64_t value, bool endian) {
	while (true) {
		uint8_t byte = value & 0x7F;
		value >>= 7;
		if (value != 0) byte |= 0x80;
		this->writeUint8(byte);
		if (value == 0) break;
	}
}

void BinStream::writeUint8(uint8_t value, bool endian) {
	if (this->position + 1 > this->size) {
		return;
	}

	this->data[this->position] = value;
	this->position += 1;
}

void BinStream::writeUint16(uint16_t value, bool endian) {
	if (this->position + 2 > this->size) {
		return;
	}

	if (endian) {
		this->data[this->position] = (value >> 8) & 0xFF;
		this->data[this->position + 1] = value & 0xFF;
	}
	else {
		this->data[this->position] = value & 0xFF;
		this->data[this->position + 1] = (value >> 8) & 0xFF;
	}

	this->position += 2;
}

void BinStream::writeUint24(uint24_t value, bool endian) {
	if (this->position + 3 > this->size) {
		return;
	}

	if (endian) {
		this->data[this->position] = (value >> 16) & 0xFF;
		this->data[this->position + 1] = (value >> 8) & 0xFF;
		this->data[this->position + 2] = value & 0xFF;
	}
	else {
		this->data[this->position] = value & 0xFF;
		this->data[this->position + 1] = (value >> 8) & 0xFF;
		this->data[this->position + 2] = (value >> 16) & 0xFF;
	}

	this->position += 3;
}

void BinStream::writeUint32(uint32_t value, bool endian) {
	if (this->position + 4 > this->size) {
		return;
	}

	if (endian) {
		this->data[this->position] = (value >> 24) & 0xFF;
		this->data[this->position + 1] = (value >> 16) & 0xFF;
		this->data[this->position + 2] = (value >> 8) & 0xFF;
		this->data[this->position + 3] = value & 0xFF;
	}
	else {
		this->data[this->position] = value & 0xFF;
		this->data[this->position + 1] = (value >> 8) & 0xFF;
		this->data[this->position + 2] = (value >> 16) & 0xFF;
		this->data[this->position + 3] = (value >> 24) & 0xFF;
	}

	this->position += 4;
}

void BinStream::writeUint64(uint64_t value, bool endian) {
	if (this->position + 8 > this->size) {
		return;
	}

	if (endian) {
		this->data[this->position] = (value >> 56) & 0xFF;
		this->data[this->position + 1] = (value >> 48) & 0xFF;
		this->data[this->position + 2] = (value >> 40) & 0xFF;
		this->data[this->position + 3] = (value >> 32) & 0xFF;
		this->data[this->position + 4] = (value >> 24) & 0xFF;
		this->data[this->position + 5] = (value >> 16) & 0xFF;
		this->data[this->position + 6] = (value >> 8) & 0xFF;
		this->data[this->position + 7] = value & 0xFF;
	}
	else {
		this->data[this->position] = value & 0xFF;
		this->data[this->position + 1] = (value >> 8) & 0xFF;
		this->data[this->position + 2] = (value >> 16) & 0xFF;
		this->data[this->position + 3] = (value >> 24) & 0xFF;
		this->data[this->position + 4] = (value >> 32) & 0xFF;
		this->data[this->position + 5] = (value >> 40) & 0xFF;
		this->data[this->position + 6] = (value >> 48) & 0xFF;
		this->data[this->position + 7] = (value >> 56) & 0xFF;
	}

	this->position += 8;
}

void BinStream::writeVarInt(int32_t value, bool endian) {
	uint32_t zigzag = (value << 1) ^ (value >> 31);
	writeUVarInt(zigzag, endian);
}

void BinStream::writeVarLong(int64_t value, bool endian) {
	uint64_t zigzag = (value << 1) ^ (value >> 63);
	writeUVarLong(zigzag, endian);
}

void BinStream::writeInt8(int8_t value, bool endian) {
	this->writeUint8((uint8_t)value, endian);
}

void BinStream::writeInt16(int16_t value, bool endian) {
	this->writeUint16((uint16_t)value, endian);
}

void BinStream::writeInt24(int24_t value, bool endian) {
	this->writeUint24((uint32_t)value, endian);
}

void BinStream::writeInt32(int32_t value, bool endian) {
	this->writeUint32((uint32_t)value, endian);
}

void BinStream::writeInt64(int64_t value, bool endian) {
	this->writeUint64((uint64_t)value, endian);
}

void BinStream::writeFloat(float value, bool endian) {
	if (this->position + 4 > this->size) {
		return;
	}

	uint32_t* ptr = (uint32_t*)&value;
	this->writeUint32(*ptr, endian);
}

void BinStream::writeDouble(double value, bool endian) {
	if (this->position + 8 > this->size) {
		return;
	}

	uint64_t* ptr = (uint64_t*)&value;
	this->writeUint64(*ptr, endian);
}

uint8_t BinStream::readUint8(bool endian) {
	if (this->position + 1 > this->size) {
		return 0;
	}

	uint8_t value = this->data[this->position];
	this->position += 1;
	return value;
}

uint16_t BinStream::readUint16(bool endian) {
	if (this->position + 2 > this->size) {
		return 0;
	}

	uint16_t value;
	if (endian) {
		value = (this->data[this->position] << 8) | this->data[this->position + 1];
	}
	else {
		value = (this->data[this->position + 1] << 8) | this->data[this->position];
	}

	this->position += 2;
	return value;
}

uint24_t BinStream::readUint24(bool endian) {
	if (this->position + 3 > this->size) {
		return 0;
	}

	uint32_t value;
	if (endian) {
		value = (this->data[this->position] << 16) | (this->data[this->position + 1] << 8) | this->data[this->position + 2];
	}
	else {
		value = this->data[this->position] | (this->data[this->position + 1] << 8) | (this->data[this->position + 2] << 16);
	}

	this->position += 3;
	return value;
}

uint32_t BinStream::readUint32(bool endian) {
	if (this->position + 4 > this->size) {
		return 0;
	}

	uint32_t value;
	if (endian) {
		value = (this->data[this->position] << 24) | (this->data[this->position + 1] << 16) | (this->data[this->position + 2] << 8) | this->data[this->position + 3];
	}
	else {
		value = this->data[this->position] | (this->data[this->position + 1] << 8) | (this->data[this->position + 2] << 16) | (this->data[this->position + 3] << 24);
	}

	this->position += 4;
	return value;
}

uint64_t BinStream::readUint64(bool endian) {
	if (this->position + 8 > this->size) {
		return 0;
	}

	uint64_t value;
	if (endian) {
		value = ((uint64_t)this->data[this->position] << 56) | ((uint64_t)this->data[this->position + 1] << 48) | ((uint64_t)this->data[this->position + 2] << 40) | ((uint64_t)this->data[this->position + 3] << 32) | ((uint64_t)this->data[this->position + 4] << 24) | ((uint64_t)this->data[this->position + 5] << 16) | ((uint64_t)this->data[this->position + 6] << 8) | (uint64_t)this->data[this->position + 7];
	}
	else {
		value = (uint64_t)this->data[this->position] | ((uint64_t)this->data[this->position + 1] << 8) | ((uint64_t)this->data[this->position + 2] << 16) | ((uint64_t)this->data[this->position + 3] << 24) | ((uint64_t)this->data[this->position + 4] << 32) | ((uint64_t)this->data[this->position + 5] << 40) | ((uint64_t)this->data[this->position + 6] << 48) | ((uint64_t)this->data[this->position + 7] << 56);
	}

	this->position += 8;
	return value;
}

int8_t BinStream::readInt8(bool endian) {
	return (int8_t)this->readUint8(endian);
}

int16_t BinStream::readInt16(bool endian) {
	return (int16_t)this->readUint16(endian);
}

int24_t BinStream::readInt24(bool endian) {
	return (int32_t)this->readUint24(endian);
}

int32_t BinStream::readInt32(bool endian) {
	return (int32_t)this->readUint32(endian);
}

int64_t BinStream::readInt64(bool endian) {
	return (int64_t)this->readUint64(endian);
}

float BinStream::readFloat(bool endian) {
	uint32_t value = this->readUint32(endian);
	return *(float*)&value;
}

double BinStream::readDouble(bool endian) {
	uint64_t value = this->readUint64(endian);
	return *(double*)&value;
}
void BinStream::writeString(std::string value, bool endian) {
	this->writeUVarInt(value.length(), endian);
	this->writeBytes((uint8_t*)(value.c_str()), value.length(), endian);
}

void BinStream::writeString16(std::string value, bool endian) {
	this->writeUint16(value.length(), endian);
	this->writeBytes((uint8_t*)(value.c_str()), value.length(), endian);
}

std::string BinStream::readString(bool endian) {
	uint32_t length = this->readUVarInt(endian);
	uint8_t* data = this->readBytes(length, endian);
	std::string value((char*)data, length);
	delete[] data;
	return value;
}

void BinStream::writeBytes(const uint8_t* value, uint32_t length, bool endian) {
	if (this->position + length > this->size) {
		std::cout << "Failed to write bytes: Out of bounds\n";
		return;
	}

	memcpy(this->data + this->position, value, length);
	this->position += length;
}

uint8_t* BinStream::readBytes(uint32_t length, bool endian) {
	if (this->position + length > this->size) {
		return nullptr;
	}

	uint8_t* value = new uint8_t[length];
	memcpy(value, this->data + this->position, length);
	this->position += length;
	return value;
}

void BinStream::writeBinStream(BinStream value, bool endian) {
	this->writeUint32(value.getSize(), endian);
	this->writeBytes(value.getData(), value.getSize(), endian);
}

BinStream BinStream::readBinStream(bool endian) {
	uint32_t length = this->readUint32(endian);
	uint8_t* data = this->readBytes(length, endian);
	BinStream value(data, length);
	delete[] data;
	return value;
}

void BinStream::reset() {
	this->position = 0;
}

void BinStream::clear() {
	delete[] this->data;
	this->data = nullptr;
	this->size = 0;
	this->position = 0;
}

std::string BinStream::print() {
	std::stringstream ss;
	ss << "Bin " << this->size << ": ";
	for (uint32_t i = 0; i < this->size; i++) {
		ss << std::hex << std::setw(2) << std::uppercase << (short)this->data[i] << " ";
	}
	return ss.str();
}