#pragma once
#include <vector>
#include <stdint.h>
#include <iostream>

enum class SHA_3_Length {SHA_224, SHA_256, SHA_384, SHA_512};

class SHA_3 {
public :
	explicit SHA_3(const SHA_3_Length sha_length);

	std::vector<uint8_t> hashing(const std::vector<uint8_t>& message);

private:
	unsigned int output_size;
	unsigned int r;
	unsigned int c;

	const int rot_offset[25] = {
		0, 1, 62, 28, 27,
		36, 44, 6, 55, 20,
		3, 10, 43, 25, 39,
		41, 45, 15, 21, 8,
		18, 2, 61, 56, 14
	};

	const uint64_t round_constants[24] = { 
		0x0000000000000001, 0x0000000000008082, 0x800000000000808a, 0x8000000080008000,
		0x000000000000808b, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009,
		0x000000000000008a, 0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
		0x000000008000808b, 0x800000000000008b, 0x8000000000008089, 0x8000000000008003,
		0x8000000000008002, 0x8000000000000080, 0x000000000000800a, 0x800000008000000a,
		0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008 
	};

	void keccak(uint64_t* state);
	void padding(std::vector<uint8_t>& message, int block_size);
	void xor_vectors(uint64_t* vec64, std::vector<uint8_t>& vec8);
	std::vector<uint8_t> pop_front_block(std::vector<uint8_t>& vector, int& block_size);

	void keccak_theta(uint64_t* A);
	void keccak_ro(uint64_t* A);
	void keccak_pi(uint64_t* A);
	void keccak_chi(uint64_t* A);
	void keccak_yot(uint64_t* A, size_t& round);

	uint64_t right_rotate(uint64_t value, size_t offset) { return value >> offset | value << (sizeof(uint64_t) * 8 - offset); };
	uint64_t left_rotate(uint64_t value, size_t offset) { return value << offset | value >> (sizeof(uint64_t) * 8 - offset); };

	std::vector<uint8_t> get_hash(uint64_t* state);
};