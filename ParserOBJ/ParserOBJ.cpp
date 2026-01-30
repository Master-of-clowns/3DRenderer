//
// Created by s24b_ on 30.01.2026.
//

#include "ParserOBJ.h"
#include <algorithm>
#include <cctype>

std::vector<Triangle> ParserOBJ::Parse(const std::string& filename){
	std::vector<Triangle> triangles;
	std::vector<Vector3d> vertices;

	std::ifstream file(filename);
	if (!file.is_open()){
		std::cerr << "Could not open file " << filename << std::endl;
		return triangles;
	}

	std::string line;
	int lineNum = 0;

	while (std::getline(file, line)){
		lineNum++;

		size_t commentPos = line.find('#');
		if (commentPos != std::string::npos){
			line = line.substr(0, commentPos);
		}

		line.erase(std::remove_if(line.begin(), line.end(),
								[](unsigned char c){ return std::isspace(c) && c != ' '; }),
					line.end());

		if (line.empty()) continue;

		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v"){
			ParseVertex(line, vertices);
		} else if (prefix == "f"){
			ParseFace(line, vertices, triangles);
		}
	}

	file.close();
	return triangles;
}

void ParserOBJ::ParseVertex(const std::string& line, std::vector<Vector3d>& vertices){
	std::istringstream iss(line);
	std::string prefix;
	double x, y, z;

	iss >> prefix >> x >> y >> z;
	vertices.push_back(Vector3d(x, y, z));
}

void ParserOBJ::ParseFace(const std::string& line,
						const std::vector<Vector3d>& vertices,
						std::vector<Triangle>& triangles){
	std::vector<std::string> parts = Split(line, ' ');

	if (parts.size() < 4){
		std::cerr << "Face with less than 3 vertices" << std::endl;
		return;
	}

	std::vector<int> vertexIndices;

	for (size_t i = 1; i < parts.size(); i++){
		std::vector<std::string> subparts = Split(parts[i], '/');

		if (!subparts.empty()){
			try{
				int idx = std::stoi(subparts[0]);
				if (idx > 0){
					vertexIndices.push_back(idx - 1);
				} else if (idx < 0){
					vertexIndices.push_back(vertices.size() + idx);
				}
			}
			catch (...){
				std::cerr << "Could not parse vertex index: " << parts[i] << std::endl;
			}
		}
	}

	if (vertexIndices.size() >= 3){
		for (size_t i = 1; i < vertexIndices.size() - 1; i++){
			int idx1 = vertexIndices[0];
			int idx2 = vertexIndices[i];
			int idx3 = vertexIndices[i + 1];

			if (idx1 >= 0 && idx1 < static_cast<int>(vertices.size()) &&
				idx2 >= 0 && idx2 < static_cast<int>(vertices.size()) &&
				idx3 >= 0 && idx3 < static_cast<int>(vertices.size())){
				triangles.push_back(Triangle(
					vertices[idx1],
					vertices[idx2],
					vertices[idx3]
				));
			} else{
				std::cerr << "Invalid vertex index in face" << std::endl;
			}
		}
	}
}

Vector3d ParserOBJ::ParseVector3(const std::string& str){
	std::istringstream iss(str);
	double x, y, z;
	iss >> x >> y >> z;
	return Vector3d(x, y, z);
}

std::vector<std::string> ParserOBJ::Split(const std::string& s, char delimiter){
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);

	while (std::getline(tokenStream, token, delimiter)){
		if (!token.empty()){
			tokens.push_back(token);
		}
	}

	return tokens;
}
