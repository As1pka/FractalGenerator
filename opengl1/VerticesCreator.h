#pragma once
#include <iostream>
#include <complex>
#include <vector>

enum struct vert_type
{
	mandelbrot,
	triangle
};


class VerticesCreator
{
public:
	VerticesCreator() { curr_type = vert_type::mandelbrot; };
	~VerticesCreator() {};

	void setType(vert_type new_type);
	void getVertices(float width, float height, std::vector<float>& vertices);

private:
	vert_type curr_type;
};

