#include "SHA-3.h"

SHA_3::SHA_3(const SHA_3_Length sha_length) {
	switch (sha_length) {
	case SHA_3_Length::SHA_224:
		r = 1152;
		c = 448;
		output_size = 224;
		break;
	case SHA_3_Length::SHA_256:
		r = 1088;
		c = 512;
		output_size = 256;
		break;
	case SHA_3_Length::SHA_384:
		r = 832;
		c = 768;
		output_size = 384;
		break;
	case SHA_3_Length::SHA_512:
		r = 576;
		c = 1024;
		output_size = 512;
		break;
	}
}

std::vector<uint8_t> SHA_3::hashing(const std::vector<uint8_t>& message) {
	//Initialization and Padding
	uint64_t state[25] = { 0 };
	int block_size = r / 8;

	std::vector<uint8_t> padded_message = message;
	padding(padded_message, block_size);
	std::vector<uint8_t> block;

	//Absorbing
	for (size_t i = 0; i < padded_message.size(); i += block_size) {
		block = pop_front_block(padded_message, block_size);
		xor_vectors(state, block);
		keccak(state);
	}

	//Squeezing
	return get_hash(state);
}

void SHA_3::keccak(uint64_t* state) {
	for (size_t round = 0; round < 24; round++) {
		keccak_theta(state);

		/*std::cout << "State theta:";
		for (int i = 0; i < 25; i++) {
			std::cout << std::hex << static_cast<int>(state[i]) << " ";
		}
		std::cout << std::endl << std::endl;*/

		keccak_ro(state);

		/*std::cout << "State ro:";
		for (int i = 0; i < 25; i++) {
			std::cout << std::hex << static_cast<int>(state[i]) << " ";
		}
		std::cout << std::endl << std::endl;*/

		keccak_pi(state);

		/*std::cout << "State pi:";
		for (int i = 0; i < 25; i++) {
			std::cout << std::hex << static_cast<int>(state[i]) << " ";
		}
		std::cout << std::endl << std::endl;*/

		keccak_chi(state);

		/*std::cout << "State chi:";
		for (int i = 0; i < 25; i++) {
			std::cout << std::hex << static_cast<int>(state[i]) << " ";
		}
		std::cout << std::endl << std::endl;*/

		keccak_yot(state, round);

		/*std::cout << "State yot:";
		for (int i = 0; i < 25; i++) {
			std::cout << std::hex << static_cast<int>(state[i]) << " ";
		}
		std::cout << std::endl << std::endl;*/
	}
}

void SHA_3::padding(std::vector<uint8_t>& message, int block_size) {
	/*updated:*/
	if (message.size() != 0 && message.size() % block_size == 0)
		return;

	if (message.size() % block_size == 1) {
		message.push_back(0x86);
		return;
	}
	/*--------*/

	message.push_back(0x06);

	while ((message.size() + 1) % block_size != 0)
		message.push_back(0x00);

	message.push_back(0x80);
}

/*updated:*/
void SHA_3::xor_vectors(uint64_t* state64, std::vector<uint8_t>& vec8) {
	size_t vec64_size = 25;
	size_t vec8_size = vec8.size();

	for (int i = 0; i < vec64_size && i * sizeof(uint64_t) < vec8_size; i++) {
		uint64_t buffer = 0;
		for (int j = 0; j < sizeof(uint64_t) && i * sizeof(uint64_t) + j < vec8_size; j++)
			buffer |= (uint64_t)vec8[i * sizeof(uint64_t) + j] << (j * 8);
		state64[i] ^= buffer;
	}
}
/*--------*/

std::vector<uint8_t> SHA_3::pop_front_block(std::vector<uint8_t>& vector, int& block_size) {
	std::vector<uint8_t> block;
	for (size_t i = 0; i < block_size; i++)
		block.push_back(vector[i]);
	vector.erase(vector.begin(), vector.begin() + block_size);
	
	/*updated:*/
	while (block.size() < 25)
		block.push_back(0x00);
	/*--------*/

	return block;
}

void SHA_3::keccak_theta(uint64_t* A) {
	uint64_t C[5], D[5];

	for (size_t i = 0; i < 5; i++)
		C[i] = A[i] ^ A[i + 5] ^ A[i + 10] ^ A[i + 15] ^ A[i + 20];

	D[0] = left_rotate(C[1], 1) ^ C[4];
	D[1] = left_rotate(C[2], 1) ^ C[0];
	D[2] = left_rotate(C[3], 1) ^ C[1];
	D[3] = left_rotate(C[4], 1) ^ C[2];
	D[4] = left_rotate(C[0], 1) ^ C[3];

	for (size_t i = 0; i < 5; i++) {
		A[i] ^= D[i];
		A[i + 5] ^= D[i];
		A[i + 10] ^= D[i];
		A[i + 15] ^= D[i];
		A[i + 20] ^= D[i];
	}
}

void SHA_3::keccak_ro(uint64_t* A) {
	for (size_t i = 1; i < 25; i++)
		A[i] = left_rotate(A[i], rot_offset[i]);
}

void SHA_3::keccak_pi(uint64_t* A) {
	uint64_t A1 = A[1];
	A[1] = A[6], A[6] = A[9], A[9] = A[22], A[22] = A[14],
	A[14] = A[20], A[20] = A[2], A[2] = A[12], A[12] = A[13],
	A[13] = A[19], A[19] = A[23], A[23] = A[15], A[15] = A[4],
	A[4] = A[24], A[24] = A[21], A[21] = A[8], A[8] = A[16],
	A[16] = A[5], A[5] = A[3], A[3] = A[18], A[18] = A[17],
	A[17] = A[11], A[11] = A[7], A[7] = A[10], A[10] = A1;
}

void SHA_3::keccak_chi(uint64_t* A) {
	for (size_t i = 0; i < 25; i += 5) {
		uint64_t A0 = A[0 + i], A1 = A[1 + i];
		A[0 + i] ^= ~A1 & A[2 + i];
		A[1 + i] ^= ~A[2 + i] & A[3 + i];
		A[2 + i] ^= ~A[3 + i] & A[4 + i];
		A[3 + i] ^= ~A[4 + i] & A0;
		A[4 + i] ^= ~A0 & A1;
	}
}

void SHA_3::keccak_yot(uint64_t* A, size_t& round) {
	A[0] ^= round_constants[round];
}

/*updated:*/
std::vector<uint8_t> SHA_3::get_hash(uint64_t* state) {
	std::vector<uint8_t> hash;
	for (size_t i = 0; i < (c / 2) / 64; i++) {
		const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&state[i]);
		for (int j = 0; j < sizeof(uint64_t); j++)
			hash.push_back(bytes[j]);
	}
	return hash;
}
/*--------*/