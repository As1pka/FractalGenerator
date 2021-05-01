#include "VerticesCreator.h"
#include "rgb_hsv_converter.h"

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
        // Эта настройка для обрезки мандельброта
        //for (int x = int(width*0.37); x < int(width*0.87); x++)
        {
            for (float y = 0; y < height; y++)
            //for (int y = 0; y < 1; y++)
            // Эта настройка для обрезки мандельброта
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
        /*for (int i = 0; i < vertices.size();)
        {
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << '\n';
        }*/
        std::cout << vertices.size() << '\n';
        break;
	}
    case(vert_type::mandelbrot_parallel):
    {
        std::mutex m_push;
        const int cpu_threads_count = std::thread::hardware_concurrency();
        //for (float x = 0; x < width; x++)
        std::vector<std::thread> thread_array;
        for (float x = 0; x < width; x++)
            // Эта настройка для обрезки мандельброта
            //for (int x = int(width*0.37); x < int(width*0.87); x++)
        {
            for (float y = 0; y < height;)
            {
                for (int i = 0; i < cpu_threads_count && y < height; i++, y++) {
                    std::thread th(&VerticesCreator::putVerticesInParallel, this, std::ref(vertices), std::ref(m_push), x, y);
                    thread_array.push_back(std::move(th));
                    //thread_array[i] = std::thread(putVerticesInParallel(), ref(vertices), ref(m_push), x, y);
                } 
                for (int i = thread_array.size() - 1; i > -1; i--) {
                    //if (thread_array[i].joinable()) {
                    thread_array[i].join();
                    thread_array.pop_back();
                    //}
                }
                /*for (int thr = 0; thr < cpu_threads_count && y < height; y++)
                {
                    std::thread push_thread(putVerticesInParallel(), ref(vertices), ref(m_push), x, y);
                }                

                const int length = 10;
                std::thread::id id;
                std::thread thread_array[length];
                int res_arr[length] = { 0 };
                for (int i = 0; i < length; i++) {
                    thread_array[i] = std::thread();
                }*/
            }
        }
        /*for (int i = 0; i < vertices.size();)
        {
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << '\n';
        }*/
        std::cout << vertices.size() << '\n';
        break;
    }
    case(vert_type::mandelbrot_wo_bg):
    {
        rgb bg_color;
        //for (float x = 0; x < width; x++)
        // Эта настройка для обрезки мандельброта
        for (int x = int(width*0.37); x < int(width*0.87); x++)
        {
            //for (float y = 0; y < height; y++)
                //for (int y = 0; y < 1; y++)
            // Эта настройка для обрезки мандельброта
            for (int y = 0; y < int(height*0.5); y++)
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
                        // Первый элемент заранее известно что имеет цвет фона
                        if (x == y && x == 0.f)
                        {
                            bg_color = out_color;
                        }
                        // Если текущий цвет отличается от цвета фона, то добавляем вершину
                        else if (bg_color != out_color)
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
                    //vertices.push_back(255.f);
                    vertices.push_back(0.f);
                    vertices.push_back(0.f);
                    vertices.push_back(0.f);
                }
            }
        }
        /*for (int i = 0; i < vertices.size();)
        {
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << ' ';
            std::cout << vertices[i++] << '\n';
        }*/
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
