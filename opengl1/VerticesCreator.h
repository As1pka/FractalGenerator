#pragma once
#include <iostream>
#include <complex>
#include <vector>
#include <mutex> 
#include <thread> 


enum struct vert_type
{
	mandelbrot,
	mandelbrot_wo_bg, //without background
	mandelbrot_parallel,
	mandelbrot_parallel_wo_bg,
	mandelbrot_parallel_wo_bg_half_scene,
	levi2D,
	levi3D,
	triangle
};


class VerticesCreator
{
public:
	VerticesCreator() { 
		curr_type = vert_type::mandelbrot;
		width = 800.f;
		height = 800.f;
		levi_n = 9;
		levi_div = 3;
	};
	~VerticesCreator() {};

	void setType(vert_type new_type);
	void setBorder(float width, float height);
	void setLeviN(const int n);
	void setLeviDiv(const int div);

	vert_type getType();
	void getVertices(std::vector<float>& vertices);

private:
	vert_type curr_type;
	float width, height;
	int levi_n, levi_div;

	void putVerticesInParallel(std::vector<float>& vertices, std::mutex & m_push, const float x, const float y);
	void threadMandelbrot(std::vector<float>& full_vertices, std::mutex& m_push, const float start_w, const float end_w, const float start_h, const float end_h);
	void threadMandelbrotWoBg(std::vector<float>& full_vertices, std::mutex& m_push, const float start_w, const float end_w, const float start_h, const float end_h);
	void threadMandelbrotWoBgHalfScene(std::vector<float>& full_vertices, std::mutex& m_push, const float start_w, const float end_w, const float start_h, const float end_h, const float border_h);
	void nextLeviIter2D(std::vector<float>& vertices, const float xA, const float yA, const float xB, const float yB, const int n);
	void nextLeviIter3D(std::vector<float>& vertices, const float xA, const float yA, const float xB, const float yB, const int n);
};

