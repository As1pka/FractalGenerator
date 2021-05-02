#include "VerticesCreator.h"
#include "color_expand.h"

void VerticesCreator::setType(vert_type new_type)
{
	this->curr_type = new_type;
}

void VerticesCreator::setBorder(float width, float height)
{
    this->width = width;
    this->height = height;
}

void VerticesCreator::getVertices(std::vector<float>& vertices)
{
	switch (curr_type)
	{
	case(vert_type::mandelbrot):
	{
        for (float x = 0; x < width; x++)
        // ��� ��������� ��� ������� ������������
        //for (int x = int(width*0.37); x < int(width*0.87); x++)
        {
            for (float y = 0; y < height; y++)
            //for (int y = 0; y < 1; y++)
            // ��� ��������� ��� ������� ������������
            //for (int y = 0; y < int(height*0.5); y++)
            {
                bool isColorSet = false;
                vertices.push_back(-1.0f + 2.f * x / width);
                //vertices.push_back(float(-1.0f + 2.f * float(x - float(width * 0.37)) / (float(width * 0.87))));
                //vertices.push_back(float(1.0f - 2.f * float(y) / (float(height*0.5))));
                vertices.push_back(1.0f - 2.f * y / height);
                vertices.push_back(0.f);

                std::complex<float> c0((x - 0.75f * width) / (width / 4.f), (y - height / 4.f) / (height / 4.f));
                std::complex<float> c(0);
                for (int i = 1; i < 1000; i++)
                {
                    //float new_y = (y - (height / 4)) / (height / 4);
                    if (std::abs(c) > 2)
                    {
                        //vertices.push_back(float(i));
                        rgb out_color = rgb_conv(float(i));
                        vertices.push_back(float(out_color.r));
                        vertices.push_back(float(out_color.g));
                        vertices.push_back(float(out_color.b));
                        isColorSet = true;
                        break;
                    }
                    c = c * c + c0;
                }
                if (!isColorSet)
                {
                    //vertices.push_back(255.f);
                    vertices.push_back(0.f);
                    vertices.push_back(0.f);
                    vertices.push_back(0.f);
                }
            }
        }
        std::cout << vertices.size() << '\n';
        break;
	}
    case(vert_type::mandelbrot_parallel):
    {
        std::mutex m_push;
        std::vector<std::vector<float>> thread_vertices;

        const int cpu_threads_count = std::thread::hardware_concurrency();
        std::vector<std::thread> thread_array;
        float step_w = width / float(cpu_threads_count);
        //float step_h = height / float(cpu_threads_count);
        float start_w = 0;
        float start_h = 0;
        for (int i = 0; i < cpu_threads_count; i++, start_w += step_w) { //, start_h += step_h) {
            std::vector<float> i_thread_vert;
            thread_vertices.push_back(i_thread_vert);
            //std::thread th(&VerticesCreator::putVerticesInParallel, this, std::ref(vertices), std::ref(m_push), x, y);
            if (i == cpu_threads_count - 1) {
                std::thread th(&VerticesCreator::threadMandelbrot, this, std::ref(vertices), std::ref(m_push), start_w, width, start_h, height);
                thread_array.push_back(std::move(th));
            }
            else
            {
                std::thread th(&VerticesCreator::threadMandelbrot, this, std::ref(vertices), std::ref(m_push), start_w, start_w + step_w, start_h, height);
                thread_array.push_back(std::move(th));
            }
            

        }
        for (int i = 0; i < cpu_threads_count; i++) {
            if (thread_array[i].joinable()) {
                thread_array[i].join();
            }
        }        
        std::cout << vertices.size() << '\n';
        break;
    }
    case(vert_type::mandelbrot_parallel_wo_bg):
    {
        std::mutex m_push;
        std::vector<std::vector<float>> thread_vertices;

        /*const */int cpu_threads_count = std::thread::hardware_concurrency();
        std::vector<std::thread> thread_array;
        float end_w = width * 0.87f;
        float end_h = height * 0.5f;

        //float step_w = end_w / float(cpu_threads_count);
        float step_h = end_h / float(cpu_threads_count);
        //for (int x = int(width * 0.37); x < int(width * 0.87); x++)
        float start_w = width * 0.37f;
        float start_h = 0;

        //cpu_threads_count = 1;
        for (int i = 0; i < cpu_threads_count; i++, start_h += step_h) { //start_w += step_w) { //, 
            std::vector<float> i_thread_vert;
            thread_vertices.push_back(i_thread_vert);
            //std::thread th(&VerticesCreator::putVerticesInParallel, this, std::ref(vertices), std::ref(m_push), x, y);
            /*if (i == cpu_threads_count - 1) {
                std::thread th(&VerticesCreator::threadMandelbrotWoBg, this, std::ref(vertices), std::ref(m_push), start_w, end_w, start_h, end_h);
                thread_array.push_back(std::move(th));
            }
            else*/
            {
                std::thread th(&VerticesCreator::threadMandelbrotWoBg, this, std::ref(vertices), std::ref(m_push), start_w, end_w, start_h, start_h + step_h);
                //std::thread th(&VerticesCreator::threadMandelbrotWoBg, this, std::ref(vertices), std::ref(m_push), start_w, start_w + step_w, start_h, end_h);
                thread_array.push_back(std::move(th));
            }


        }
        for (int i = 0; i < cpu_threads_count; i++) {
            if (thread_array[i].joinable()) {
                thread_array[i].join();
            }
        }
        std::cout << vertices.size() << '\n';
        break;
    }
    case(vert_type::mandelbrot_wo_bg):
    {
        rgb bg_color;
        bool bg_color_set = false;
        for (float x = 0; x < width; x++)
        // ��� ��������� ��� ������� ������������
        //for (int x = int(width*0.37); x < int(width*0.87); x++)
        {
            for (float y = 0; y < height; y++)
                //for (int y = 0; y < 1; y++)
            // ��� ��������� ��� ������� ������������
            //for (int y = 0; y < int(height*0.5); y++)
            {
                bool isColorSet = false;

                std::complex<float> c0((x - 0.75f * width) / (width / 4.f), (y - height / 4.f) / (height / 4.f));
                std::complex<float> c(0);
                for (int i = 1; i < 1000; i++)
                {
                    if (std::abs(c) > 2)
                    {
                        rgb out_color = rgb_conv(float(i));
                        // ������ ������� ������� �������� ��� ����� ���� ����
                        if (!bg_color_set)
                        {
                            bg_color = out_color;
                            bg_color_set = true;
                        }
                        // ���� ������� ���� ���������� �� ����� ����, �� ��������� �������
                        //else if (bg_color != out_color)
                        else if (!isCloseColor(bg_color, out_color, 0.2))
                        {
                            vertices.push_back(-1.0f + 2.f * x / width);
                            vertices.push_back(1.0f - 2.f * y / height);
                            vertices.push_back(0.f);
                            vertices.push_back(float(out_color.r));
                            vertices.push_back(float(out_color.g));
                            vertices.push_back(float(out_color.b));
                        }                        
                        isColorSet = true;
                        break;
                    }
                    c = c * c + c0;
                }
                if (!isColorSet)
                {
                    vertices.push_back(-1.0f + 2.f * x / width);
                    vertices.push_back(1.0f - 2.f * y / height);
                    vertices.push_back(0.f);
                    vertices.push_back(0.f);
                    vertices.push_back(0.f);
                    vertices.push_back(0.f);
                }
            }
        }
        std::cout << vertices.size() << '\n';
        break;
    }
    case(vert_type::triangle):
    {
        vertices.push_back(0.0f);
        vertices.push_back(-0.5f);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        vertices.push_back(-0.5f);
        vertices.push_back(-0.5f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);

        vertices.push_back(0.0f);
        vertices.push_back(0.5f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        break;
    }
	default:
		break;
	}
}

void VerticesCreator::putVerticesInParallel(std::vector<float>& vertices, std::mutex & m_push, const float x, const float y)
{
    bool isColorSet = false;

    std::complex<float> c0((x - 0.75f * width) / (width / 4.f), (y - height / 4.f) / (height / 4.f));
    std::complex<float> c(0);
    for (int i = 1; i < 1000; i++)
    {
        if (std::abs(c) > 2)
        {
            rgb out_color = rgb_conv(float(i));
            m_push.lock();
            vertices.push_back(-1.0f + 2.f * x / width);
            vertices.push_back(1.0f - 2.f * y / height);
            vertices.push_back(0.f);
            vertices.push_back(float(out_color.r));
            vertices.push_back(float(out_color.g));
            vertices.push_back(float(out_color.b));
            m_push.unlock();
            isColorSet = true;
            break;
        }
        c = c * c + c0;
    }
    if (!isColorSet)
    {
        m_push.lock();
        vertices.push_back(-1.0f + 2.f * x / width);
        vertices.push_back(1.0f - 2.f * y / height);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        m_push.unlock();
    }
}

void VerticesCreator::threadMandelbrot(std::vector<float>& full_vertices, std::mutex& m_push, const float start_w, const float end_w, const float start_h, const float end_h)
{
    std::vector<float> vertices;
    for (float x = start_w; x < end_w; x++)
    {
        for (float y = start_h; y < end_h; y++)
        {
            bool isColorSet = false;
            vertices.push_back(-1.0f + 2.f * x / width);
            vertices.push_back(1.0f - 2.f * y / height);
            vertices.push_back(0.f);

            std::complex<float> c0((x - 0.75f * width) / (width / 4.f), (y - height / 4.f) / (height / 4.f));
            std::complex<float> c(0);
            for (int i = 1; i < 1000; i++)
            {
                if (std::abs(c) > 2)
                {
                    rgb out_color = rgb_conv(float(i));
                    vertices.push_back(float(out_color.r));
                    vertices.push_back(float(out_color.g));
                    vertices.push_back(float(out_color.b));
                    isColorSet = true;
                    break;
                }
                c = c * c + c0;
            }
            if (!isColorSet)
            {
                vertices.push_back(0.f);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
            }
        }
    }
    std::cout << vertices.size() << '\n';
    m_push.lock();
    full_vertices.insert(full_vertices.end(), vertices.begin(), vertices.end());
    m_push.unlock();
}

void VerticesCreator::threadMandelbrotWoBg(std::vector<float>& full_vertices, std::mutex& m_push, const float start_w, const float end_w, const float start_h, const float end_h)
{
    static rgb bg_color;
    static bool bg_color_set = false;
    std::vector<float> vertices;
    //for (float x = start_w; x < end_w; x++)
    for (float y = start_h; y < end_h; y++)
    {
        for (float x = start_w; x < end_w; x++)
        //for (float y = start_h; y < end_h; y++)
        {
            bool isColorSet = false;

            std::complex<float> c0((x - 0.75f * width) / (width / 4.f), (y - height / 4.f) / (height / 4.f));
            std::complex<float> c(0);
            for (int i = 1; i < 1000; i++)
            {
                if (std::abs(c) > 2)
                {
                    rgb out_color = rgb_conv(float(i));
                    // ������ ������� ������� �������� ��� ����� ���� ����
                    if (!bg_color_set)
                    {
                        m_push.lock();
                        bg_color = out_color;
                        bg_color_set = true;
                        m_push.unlock();
                    }
                    // ���� ������� ���� ���������� �� ����� ����, �� ��������� �������
                    //else if (bg_color != out_color)
                    else if (!isCloseColor(bg_color, out_color, 0.2))
                    {
                        vertices.push_back(-1.0f + 2.f * x / width);
                        vertices.push_back(1.0f - 2.f * y / height);
                        vertices.push_back(0.f);
                        vertices.push_back(float(out_color.r));
                        vertices.push_back(float(out_color.g));
                        vertices.push_back(float(out_color.b));
                    }
                    isColorSet = true;
                    break;
                }
                c = c * c + c0;
            }
            if (!isColorSet)
            {
                vertices.push_back(-1.0f + 2.f * x / width);
                vertices.push_back(1.0f - 2.f * y / height);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
            }
        }
    }
    std::cout << vertices.size() << '\n';
    m_push.lock();
    full_vertices.insert(full_vertices.end(), vertices.begin(), vertices.end());
    m_push.unlock();
}
