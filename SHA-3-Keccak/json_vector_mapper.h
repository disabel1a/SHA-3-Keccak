#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

inline std::string to_underscore_case(const std::string& str) {
    std::string underscored_str = str;
    for (char& ch : underscored_str)
        if (ch == ' ')
            ch = '_';
    return underscored_str;
}

template <typename T>
inline void vector_to_json(std::vector<T>& vector, const std::string& name, const std::string& file_directory) {
    //size_t pos = file_path.find_last_of("/\\");
    std::string directory = file_directory;
    directory += "/";
    //std::string filename = file_path.substr(pos + 1);

    //pos = filename.find_last_of(".");
    //filename = filename.substr(0, pos);

    //directory += name.substr(0, name.find_first_of(' ')) + '/';

    std::string filename = to_underscore_case(name);
    filename += ".json";

    std::string json_file_path = directory + filename;

    std::ofstream file(json_file_path);

    if (!file.is_open())
        throw std::runtime_error("JSON-MAPPER: Failed to open the file!");

    std::cout << "Created: " << json_file_path << std::endl;

    file << "{\n";
    file << "  \"name\": \"" << name << "\",\n";

    file << "  \"values\": [\n";

    for (size_t i = 0; i < vector.size(); ++i) {
        file << "    " << vector[i];
        if (i != vector.size() - 1) {
            file << ",";
        }
        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    file.close();
}