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
	triangle
};


class VerticesCreator
{
public:
	VerticesCreator() { 
		curr_type = vert_type::mandelbrot;
		width = 800.f;
		height = 800.f;
	};
	~VerticesCreator() {};

	void setType(vert_type new_type);
	void setBorder(float width, float height);
	void getVertices(std::vector<float>& vertices);

private:
	vert_type curr_type;
	float width, height;

	void putVerticesInParallel(std::vector<float>& vertices, std::mutex & m_push, const float x, const float y);
	void threadMandelbrot(std::vector<float>& full_vertices, std::mutex& m_push, const float start_w, const float end_w, const float start_h, const float end_h);
	void threadMandelbrotWoBg(std::vector<float>& full_vertices, std::mutex& m_push, const float start_w, const float end_w, const float start_h, const float end_h);
	void threadMandelbrotWoBgHalfScene(std::vector<float>& full_vertices, std::mutex& m_push, const float start_w, const float end_w, const float start_h, const float end_h, const float border_h);
};

