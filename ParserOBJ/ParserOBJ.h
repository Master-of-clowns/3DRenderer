//
// Created by s24b_ on 30.01.2026.
//

#ifndef PARSEROBJ_H
#define PARSEROBJ_H

#include "../Figures/Triangle.h"
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ParserOBJ{
public:
	static std::vector<Triangle> Parse(const std::string& filename);

private:
	static void ParseVertex(const std::string& line, std::vector<Eigen::Vector3d>& vertices);

	static void ParseFace(const std::string& line,
						const std::vector<Eigen::Vector3d>& vertices,
						std::vector<Triangle>& triangles);

	static Eigen::Vector3d ParseVector3(const std::string& str);

	static std::vector<std::string> Split(const std::string& s, char delimiter);
};


#endif //PARSEROBJ_H
