#include "SHA-3.h"
#include <iostream>
#include <string>
#include <ctime>
#include <unordered_map>
#include "json_vector_mapper.h"

std::vector<uint8_t> generate_word() {
	std::vector<uint8_t> output;
	int len = 32;
	for (int i = 0; i < len; i++)
		output.push_back(rand() % 256);
	return output;
}

std::vector<uint8_t> string_to_vector(std::string& message) {
	std::vector<uint8_t> vector;
	for (char& ch : message)
		vector.push_back((uint8_t)ch);
	return vector;
}

uint16_t get_bits(uint8_t& first, uint8_t& second, int& size) {
	uint16_t result = 0;

	if (size == 8)
		return result |= first;

	result |= first;
	result << 8;
	result |= second;
	return result >> (16 - size);
}

//std::vector<int> find_prototype(SHA_3& sha_3, uint8_t& start_byte) {
//	std::vector<int> steps;
//	uint8_t main_byte = start_byte;
//	//uint8_t main_byte = sha_3.hashing(key_word)[0];
//	uint8_t tmp_byte = 0;
//	int counter = 0;
//	for (size_t i = 0; i < 1000; i++) {
//		do {
//			tmp_byte = sha_3.hashing(generate_word())[0];
//			counter++;
//		} while (main_byte != tmp_byte);
//		steps.push_back(counter);
//		counter = 0;
//	}
//	return steps;
//}

std::vector<int> find_prototype(SHA_3& sha_3, uint16_t& start_byte, int size_of_bits) {
	std::vector<int> steps;
	std::vector<uint8_t> hash;
	uint16_t main_byte = start_byte;
	//uint8_t main_byte = sha_3.hashing(key_word)[0];
	uint16_t tmp_byte = 0;
	int counter = 0;
	for (size_t i = 0; i < 1000; i++) {
		do {
			hash = sha_3.hashing(generate_word());
			tmp_byte = get_bits(hash[0], hash[1], size_of_bits);
			counter++;
		} while (main_byte != tmp_byte);
		steps.push_back(counter);
		counter = 0;
	}
	return steps;
}

//std::vector<int> find_collision(SHA_3& sha_3, uint8_t& start_byte) {
//	std::vector<int> steps;
//	std::vector<uint8_t> word;
//
//	std::unordered_map<uint8_t, int> hash_map;
//	hash_map.insert({start_byte, 0});
//
//	uint8_t tmp_byte = 0;
//	int counter = 0;
//
//	for (size_t i = 0; i < 1000; i++) {
//		word = generate_word();
//		tmp_byte = sha_3.hashing(word)[0];
//		counter++;
//
//		while (hash_map.count(tmp_byte) == 0) {
//			hash_map.insert({ tmp_byte, counter });
//			word = generate_word();
//			tmp_byte = sha_3.hashing(word)[0];
//			counter++;
//		}
//		steps.push_back(counter - hash_map[tmp_byte]);
//		hash_map.clear();
//		hash_map.insert({ start_byte, 0 });
//		counter = 0;
//		tmp_byte = 0;
//	}
//	return steps;
//}

std::vector<int> find_collision(SHA_3& sha_3, uint16_t& start_byte, int size_of_bits) {
	std::vector<int> steps;
	std::vector<uint8_t> hash;

	std::unordered_map<uint16_t, int> hash_map;
	hash_map.insert({ start_byte, 0 });
	//std::cout << start_byte << " ";

	uint8_t tmp_byte = 0;
	int counter = 0;

	for (size_t i = 0; i < 1000; i++) {
		hash = sha_3.hashing(generate_word());
		tmp_byte = get_bits(hash[0], hash[1], size_of_bits);
		counter++;
		//std::cout << tmp_byte << " ";

		while (hash_map.count(tmp_byte) == 0) {
			hash_map.insert({ tmp_byte, counter });
			hash = sha_3.hashing(generate_word());
			tmp_byte = get_bits(hash[0], hash[1], size_of_bits);
			counter++;
			//std::cout << tmp_byte << " ";
		}
		//std::cout << std::endl << "{" << tmp_byte << "}" << std::endl;
		steps.push_back(counter - hash_map[tmp_byte]);
		hash_map.clear();
		hash_map.insert({ start_byte, 0 });
		counter = 0;
		tmp_byte = 0;
	}
	return steps;
}

//void analys_func() {
//	SHA_3 sha_256(SHA_3_Length::SHA_256);
//
//	std::string zeros = "0000";
//	std::string fox = "fox";
//	std::string hello = "hello";
//
//	std::vector<int> results;
//	std::vector<uint8_t> vector;
//
//	/// zeros
//
//	vector = string_to_vector(zeros);
//
//	results = find_prototype(sha_256, vector[0]);
//
//	std::cout << "Message: {" << zeros << "}" << std::endl;
//	std::cout << "Prototype: " << std::endl;
//	for (int val : results)
//		std::cout << val << " ";
//	std::cout << std::endl;
//
//	results = find_collision(sha_256, vector[0]);
//
//	std::cout << "Collision: " << std::endl;
//	for (int val : results)
//		std::cout << val << " ";
//	std::cout << std::endl;
//
//	/// fox
//	
//	vector = string_to_vector(fox);
//
//	results = find_prototype(sha_256, vector[0]);
//
//	std::cout << "Message: {" << fox << "}" << std::endl;
//	std::cout << "Prototype: " << std::endl;
//	for (int val : results)
//		std::cout << val << " ";
//	std::cout << std::endl;
//
//	results = find_collision(sha_256, vector[0]);
//
//	std::cout << "Collision: " << std::endl;
//	for (int val : results)
//		std::cout << val << " ";
//	std::cout << std::endl;
//
//	/// hello
//
//	vector = string_to_vector(hello);
//
//	results = find_prototype(sha_256, vector[0]);
//
//	std::cout << "Message: {" << hello << "}" << std::endl;
//	std::cout << "Prototype: " << std::endl;
//	for (int val : results)
//		std::cout << val << " ";
//	std::cout << std::endl;
//
//	results = find_collision(sha_256, vector[0]);
//
//	std::cout << "Collision: " << std::endl;
//	for (int val : results)
//		std::cout << val << " ";
//	std::cout << std::endl;
//}

int mid_val(std::vector<int> vector) {
	int val = 0;
	for (int& v : vector)
		val += v;
	return val / vector.size();
}

void analys_func() {
	SHA_3 sha_256(SHA_3_Length::SHA_256);

	// Your JSON folder to save values
	std::string file_directory = "C:/Users/ivmak/OneDrive/Рабочий стол/SHA_3_json";

	int size_of_bits = 16;
	uint16_t first_bits = 0;

	std::string zeros = "0000";
	std::string fox = "fox";
	std::string hello = "hello";

	std::vector<int> results;
	std::vector<int> mid_prot;
	std::vector<int> mid_collision;
	std::vector<uint8_t> vector;

	std::string name = "";

	/// zeros

	vector = string_to_vector(zeros);
	vector = sha_256.hashing(vector);
	first_bits = get_bits(vector[0], vector[1], size_of_bits);

	results = find_prototype(sha_256, first_bits, size_of_bits);

	name = "Protorype " + zeros + " " + std::to_string(size_of_bits) + "bit";
	vector_to_json(results, name, file_directory);

	std::cout << "Message: {" << zeros << "}" << std::endl;
	std::cout << "Prototype: " << std::endl;
	for (int val : results)
		std::cout << val << " ";
	std::cout << std::endl;

	results = find_collision(sha_256, first_bits, size_of_bits);

	name = "Collision " + zeros + " " + std::to_string(size_of_bits) + "bit";
	vector_to_json(results, name, file_directory);

	std::cout << "Collision: " << std::endl;
	for (int val : results)
		std::cout << val << " ";
	std::cout << std::endl;

	/// fox

	vector = string_to_vector(fox);
	vector = sha_256.hashing(vector);
	first_bits = get_bits(vector[0], vector[1], size_of_bits);

	results = find_prototype(sha_256, first_bits, size_of_bits);

	name = "Protorype " + fox + " " + std::to_string(size_of_bits) + "bit";
	vector_to_json(results, name, file_directory);

	std::cout << "Message: {" << fox << "}" << std::endl;
	std::cout << "Prototype: " << std::endl;
	for (int val : results)
		std::cout << val << " ";
	std::cout << std::endl;

	results = find_collision(sha_256, first_bits, size_of_bits);

	name = "Collision " + fox + " " + std::to_string(size_of_bits) + "bit";
	vector_to_json(results, name, file_directory);

	std::cout << "Collision: " << std::endl;
	for (int val : results)
		std::cout << val << " ";
	std::cout << std::endl;

	/// hello

	vector = string_to_vector(hello);
	vector = sha_256.hashing(vector);
	first_bits = get_bits(vector[0], vector[1], size_of_bits);

	results = find_prototype(sha_256, first_bits, size_of_bits);

	name = "Protorype " + hello + " " + std::to_string(size_of_bits) + "bit";
	vector_to_json(results, name, file_directory);

	std::cout << "Message: {" << hello << "}" << std::endl;
	std::cout << "Prototype: " << std::endl;
	for (int val : results)
		std::cout << val << " ";
	std::cout << std::endl;

	results = find_collision(sha_256, first_bits, size_of_bits);

	name = "Collision " + hello + " " + std::to_string(size_of_bits) + "bit";
	vector_to_json(results, name, file_directory);

	std::cout << "Collision: " << std::endl;
	for (int val : results)
		std::cout << val << " ";
	std::cout << std::endl;
}

int main() {
	// ---------------------- Analys -----------------------------

	/*srand(time(0));
	analys_func();*/

	// ----------------------- Default strings -------------------

	SHA_3 sha_256(SHA_3_Length::SHA_256);

	//std::string message = "The quick brown fox jumps over the lazy dog.";

	std::string message = "";

	//std::cout << "Enter message: ";
	//std::cin >> message;

	std::vector<uint8_t> vector;
	for (char& ch : message)
		vector.push_back((uint8_t) ch);

	std::vector<uint8_t> hash = sha_256.hashing(vector);

	std::cout << std::endl << "Hash: ";
	for (const auto& byte : hash) {
		std::cout << std::hex << static_cast<int>(byte) << " ";
	}

	return 0;
}