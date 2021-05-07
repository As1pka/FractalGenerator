#include "VerticesCreator.h"
#include "color_expand.h"

const double pi2 = std::acos(-1)/2.f;
const double pi = std::acos(-1);

void VerticesCreator::setType(vert_type new_type)
{
	this->curr_type = new_type;
}

void VerticesCreator::setBorder(float width, float height)
{
    this->width = width;
    this->height = height;
}

void VerticesCreator::setLeviN(const int n)
{
    this->levi_n = n;
}

void VerticesCreator::setLeviDiv(const int div)
{
    this->levi_div = div;
}

vert_type VerticesCreator::getType()
{
    return this->curr_type;
}

void VerticesCreator::getVertices(std::vector<float>& vertices)
{
	switch (curr_type)
	{
	case(vert_type::mandelbrot):
	{
        //for (float x = 0; x < width; x++)
        // Эта настройка для обрезки мандельброта
        for (int x = int(width*0.37); x < int(width*0.87); x++)
        {
            //for (float y = 0; y < height; y++)
            //for (int y = 0; y < 1; y++)
            // Эта настройка для обрезки мандельброта
            for (int y = 0; y < int(height*0.5); y++)
            //for (int y = 0; y < int(height * 0.25); y++)
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
        //float end_h = height * 0.25f;
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
            if (i == cpu_threads_count - 1) {
                std::thread th(&VerticesCreator::threadMandelbrotWoBg, this, std::ref(vertices), std::ref(m_push), start_w, end_w, start_h, end_h);
                thread_array.push_back(std::move(th));
            }
            else
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
    case(vert_type::mandelbrot_parallel_wo_bg_half_scene):
    {
        std::mutex m_push;
        std::vector<std::vector<float>> thread_vertices;

        /*const */int cpu_threads_count = std::thread::hardware_concurrency();
        std::vector<std::thread> thread_array;
        float end_w = width * 0.87f;
        //float end_h = height * 0.5f;
        float end_h = height * 0.25f;

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
            if (i == cpu_threads_count - 1) {
                std::thread th(&VerticesCreator::threadMandelbrotWoBgHalfScene, this, std::ref(vertices), std::ref(m_push), start_w, end_w, start_h, end_h, end_h);
                thread_array.push_back(std::move(th));
            }
            else
            {
                std::thread th(&VerticesCreator::threadMandelbrotWoBgHalfScene, this, std::ref(vertices), std::ref(m_push), start_w, end_w, start_h, start_h + step_h, end_h);
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
        // Эта настройка для обрезки мандельброта
        //for (int x = int(width*0.37); x < int(width*0.87); x++)
        {
            for (float y = 0; y < height; y++)
                //for (int y = 0; y < 1; y++)
            // Эта настройка для обрезки мандельброта
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
                        // Первый элемент заранее известно что имеет цвет фона
                        if (!bg_color_set)
                        {
                            bg_color = out_color;
                            bg_color_set = true;
                        }
                        // Если текущий цвет отличается от цвета фона, то добавляем вершину
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
    case(vert_type::levi2D):
    {
        //0.4, 0.0, -0.4, 0.0, levi_n
        float xA = 0.4f;
        float yA = 0.f;
        float xB = -0.4f;
        float yB = 0.f;
        nextLeviIter2D(vertices, xA, yA, xB, yB, levi_n);
        break;
    }
    case(vert_type::levi3D):
    {
        //0.4, 0.0, -0.4, 0.0, levi_n
        float xA = 0.4f;
        float yA = 0.f;
        float xB = -0.4f;
        float yB = 0.f;
        //std::vector<std::vector<float>> vertices_array;
        nextLeviIter3D(vertices, xA, yA, xB, yB, levi_n);
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
                    // Первый элемент заранее известно что имеет цвет фона
                    if (!bg_color_set)
                    {
                        m_push.lock();
                        bg_color = out_color;
                        bg_color_set = true;
                        m_push.unlock();
                    }
                    // Если текущий цвет отличается от цвета фона, то добавляем вершину
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

void VerticesCreator::threadMandelbrotWoBgHalfScene(std::vector<float>& full_vertices, std::mutex& m_push, const float start_w, const float end_w, const float start_h, const float end_h, const float border_h)
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
                    // Первый элемент заранее известно что имеет цвет фона
                    if (!bg_color_set)
                    {
                        m_push.lock();
                        bg_color = out_color;
                        bg_color_set = true;
                        m_push.unlock();
                    }
                    // Если текущий цвет отличается от цвета фона, то добавляем вершину
                    //else if (bg_color != out_color)
                    else if (!isCloseColor(bg_color, out_color, 0.2))
                    {
                        // Текущая точка
                        vertices.push_back(-1.0f + 2.f * x / width);
                        vertices.push_back(1.0f - 2.f * y / height);
                        vertices.push_back(0.f);
                        vertices.push_back(float(out_color.r));
                        vertices.push_back(float(out_color.g));
                        vertices.push_back(float(out_color.b));
                        // Зеркальная точка
                        vertices.push_back(-1.0f + 2.f * x / width);
                        vertices.push_back(1.0f - 2.f * (border_h + (border_h - y)) / height);
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
                // Текущая точка
                vertices.push_back(-1.0f + 2.f * x / width);
                vertices.push_back(1.0f - 2.f * y / height);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
                // Зеркальная точка
                vertices.push_back(-1.0f + 2.f * x / width);
                vertices.push_back(1.0f - 2.f * (border_h + (border_h - y)) / height);
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

void VerticesCreator::nextLeviIter2D(std::vector<float>& vertices, const float xA, const float yA, const float xB, const float yB, const int n)
{
    if (n == 0)
    {
        // первая граница отрезка
        vertices.push_back(xA);
        vertices.push_back(yA);
        vertices.push_back(0.f);
        float zAB = 0.f;
        float angle = pi / 2.f;
        float cosa = std::cos(angle);
        float sina = std::sin(angle);
        /*vertices.push_back(xA*cosa + zAB* sina);
        vertices.push_back(yA);
        vertices.push_back(-xA* sina + zAB* cosa);*/
        vertices.push_back(255.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
        // вторая граница
        vertices.push_back(xB);
        vertices.push_back(yB);
        vertices.push_back(0.f);
        /*vertices.push_back(xB * cosa + zAB * sina);
        vertices.push_back(yB);
        vertices.push_back(-xB * sina + zAB * cosa);*/
        vertices.push_back(255.f);
        vertices.push_back(0.f);
        vertices.push_back(0.f);
    }
    else 
    {
        // usefull angles:
        // 1.8f - большая красивая кривая с множеством углов
        // 1.5f - мегатрон
        const float corr = 2.f;

        float xC = (xA + xB) / corr + (yB - yA) / corr;
        float yC = (yA + yB) / corr - (xB - xA) / corr;
        nextLeviIter2D(vertices, xA, yA, xC, yC, n - 1);
        nextLeviIter2D(vertices, xC, yC, xB, yB, n - 1);
    }
}

void VerticesCreator::nextLeviIter3D(std::vector<float>& vertices, const float xA, const float yA, const float xB, const float yB, const int n)
{
    if (n == 0)
    {
        // первая граница отрезка
        /*vertices.push_back(xA);
        vertices.push_back(yA);
        vertices.push_back(0.f);*/
        float zAB = 0.f;
        float angle = pi / float(levi_div);
        float cosa = std::cos(angle);
        float sina = std::sin(angle);

        float curr_xA = xA;
        float curr_yA = yA;
        float curr_zA = zAB;
        float curr_xB = xB;
        float curr_yB = yB;
        float curr_zB = zAB;

        for (int i = 0; i < levi_div; i++)
        {
            float xC = curr_xA * cosa + curr_zA * sina;
            float yC = curr_yA;
            float zC = -curr_xA * sina + curr_zA * cosa;

            // Первый треугольник
            vertices.push_back(curr_xB);
            vertices.push_back(curr_yB);
            vertices.push_back(curr_zB);
            vertices.push_back(255.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);

            vertices.push_back(xC);
            vertices.push_back(yC);
            vertices.push_back(zC);
            vertices.push_back(255.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);

            vertices.push_back(curr_xA);
            vertices.push_back(curr_yA);
            vertices.push_back(curr_zA);
            vertices.push_back(255.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);

            float xD = curr_xB * cosa + curr_zB * sina;
            float yD = curr_yB;
            float zD = -curr_xB * sina + curr_zB* cosa;

            // Второй треугольник
            vertices.push_back(curr_xB);
            vertices.push_back(curr_yB);
            vertices.push_back(curr_zB);
            vertices.push_back(255.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);

            vertices.push_back(xC);
            vertices.push_back(yC);
            vertices.push_back(zC);
            vertices.push_back(255.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);

            vertices.push_back(xD);
            vertices.push_back(yD);
            vertices.push_back(zD);
            vertices.push_back(255.f);
            vertices.push_back(0.f);
            vertices.push_back(0.f);

            curr_xA = xC;
            curr_yA = yC;
            curr_zA = zC;
            curr_xB = xD;
            curr_yB = yD;
            curr_zB = zD;
        }

        //vertices.push_back(xA * cosa + zAB * sina);
        //vertices.push_back(yA);
        //vertices.push_back(-xA * sina + zAB * cosa);
        //vertices.push_back(255.f);
        //vertices.push_back(0.f);
        //vertices.push_back(0.f);
        //// вторая граница
        //
        //vertices.push_back(xB * cosa + zAB * sina);
        //vertices.push_back(yB);
        //vertices.push_back(-xB * sina + zAB * cosa);
        //vertices.push_back(255.f);
        //vertices.push_back(0.f);
        //vertices.push_back(0.f);
    }
    else
    {
        // usefull angles:
        // 1.8f - большая красивая кривая с множеством углов
        // 1.5f - мегатрон
        // 2.5f - более острый со сторон цветок
        const float angle = 2.f;

        float xC = (xA + xB) / angle + (yB - yA) / angle;
        float yC = (yA + yB) / angle - (xB - xA) / angle;
        nextLeviIter3D(vertices, xA, yA, xC, yC, n - 1);
        nextLeviIter3D(vertices, xC, yC, xB, yB, n - 1);
    }
}
