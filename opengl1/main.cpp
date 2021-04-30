#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"
#include "rgb_hsv_converter.h"

#include <iostream>
#include <complex>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Константы
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif

    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Компилирование нашей шейдерной программы
    Shader ourShader("shader.vs", "shader.fs");  // путь к файлам шейдеров

    //Указание вершин (и буфера(ов)) и настройка вершинных атрибутов
    //float vertices[] = {
    //    // координаты       // цвета
    //    0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // нижняя правая вершина
    //   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // нижняя левая вершина
    //    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // верхняя вершина
    //};
    /*std::vector<float> vertices;
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
    vertices.push_back(1.0f);*/

    //const int tmp_size = (int)SCR_HEIGHT * (int)SCR_WIDTH;//(int)SCR_HEIGHT * (int)SCR_WIDTH * 6;
    //float vertices[tmp_size] = {};
    //int j = 0;
    //for (int x = 0; x < SCR_WIDTH; x++)
    //{
    //    //float new_x = (x - (0.75 * SCR_WIDTH)) / (SCR_WIDTH / 4);
    //    //for (int y = SCR_HEIGHT/2.; y < SCR_HEIGHT/2.+100; y++)
    //    for (int y = 0; y < SCR_HEIGHT; y++)
    //    {
    //        bool isColorSet = false;
    //        vertices[j++] = (float(-1.0f + float(x) / ((float)SCR_WIDTH) / 2.));
    //        vertices[j++] = (float(1.0f - float(y) / ((float)SCR_HEIGHT) / 2.));
    //        vertices[j++] = 0.0f;
    //        /*vertices.push_back(float(-1.0f + float(x)/((float)SCR_WIDTH)/2.));
    //        vertices.push_back(float(1.0f - float(y) / ((float)SCR_HEIGHT) / 2.));
    //        vertices.push_back(0.f);*/
    //        /*float wdt = float(SCR_WIDTH);
    //        float wdt2 = (float)SCR_WIDTH;
    //        float a = (float)x - (0.75 * float(SCR_WIDTH));
    //        float b = (float)SCR_WIDTH / 4.;
    //        float new_x = (float)x - (0.75 * float(SCR_WIDTH)) / ((float)SCR_WIDTH / 4.);*/

    //        //std::complex<float> c0(float((float)x - (0.75 * float(SCR_WIDTH))) / ((float)SCR_WIDTH / 4.), float((float)y - ((float)SCR_HEIGHT / 4.)) / ((float)SCR_HEIGHT / 4.));
    //        //std::complex<float> c(0);
    //        //for (int i = 1; i < 1000; i++)
    //        //{
    //        //    //float new_y = (y - (SCR_HEIGHT / 4)) / (SCR_HEIGHT / 4);
    //        //    if (std::abs(c) > 2)
    //        //    {
    //        //        //vertices.push_back(float(i));
    //        //        rgb out_color = rgb_conv(float(i));
    //        //        vertices.push_back(float(out_color.r));
    //        //        vertices.push_back(float(out_color.g));
    //        //        vertices.push_back(float(out_color.b));
    //        //        /*std::cout << i << '\n';
    //        //        std::cout << float(out_color.r) << ' ' << float(out_color.g) << ' ' << float(out_color.b) << '\n';*/
    //        //        /*vertices.push_back(float(255));
    //        //        vertices.push_back(float(0));
    //        //        vertices.push_back(float(0));*/
    //        //        isColorSet = true;
    //        //        break;
    //        //    }
    //        //    c = c * c + c0;
    //        //}
    //        if (!isColorSet)
    //        {
    //            vertices[j++] = (255.f);
    //            vertices[j++] = (0.f);
    //            vertices[j++] = (0.f);
    //            /*vertices.push_back(255.f);
    //            vertices.push_back(0.f);
    //            vertices.push_back(0.f);*/
    //        }
    //    }
    //}

    std::vector<float> vertices;
    for (int x = 0; x < SCR_WIDTH; x++)
    //for (int x = SCR_WIDTH/2.; x < SCR_WIDTH/2.+30; x++)
    {
        //float new_x = (x - (0.75 * SCR_WIDTH)) / (SCR_WIDTH / 4);
        for (int y = 0; y < SCR_HEIGHT; y++)
        //for (int y = 0; y < 1; y++)
        //for (int y = SCR_HEIGHT/2.; y < SCR_HEIGHT/2.+100; y++)
        {
            bool isColorSet = false;
            vertices.push_back(float(-1.0f + 2.f*float(x)/((float)SCR_WIDTH)));
            vertices.push_back(float(1.0f - 2.f * float(y) / ((float)SCR_HEIGHT)));
            vertices.push_back(0.f);
            /*float wdt = float(SCR_WIDTH);
            float wdt2 = (float)SCR_WIDTH;
            float a = (float)x - (0.75 * float(SCR_WIDTH));
            float b = (float)SCR_WIDTH / 4.;
            float new_x = (float)x - (0.75 * float(SCR_WIDTH)) / ((float)SCR_WIDTH / 4.);*/

            std::complex<float> c0(float((float)x - (0.75 * float(SCR_WIDTH))) / ((float)SCR_WIDTH / 4.), float((float)y - ((float)SCR_HEIGHT / 4.)) / ((float)SCR_HEIGHT / 4.));
            std::complex<float> c(0);
            for (int i = 1; i < 1000; i++)
            {
                //float new_y = (y - (SCR_HEIGHT / 4)) / (SCR_HEIGHT / 4);
                if (std::abs(c) > 2)
                {
                    //vertices.push_back(float(i));
                    rgb out_color = rgb_conv(float(i));
                    vertices.push_back(float(out_color.r));
                    vertices.push_back(float(out_color.g));
                    vertices.push_back(float(out_color.b));
                    /*std::cout << i << '\n';
                    std::cout << float(out_color.r) << ' ' << float(out_color.g) << ' ' << float(out_color.b) << '\n';*/
                    /*vertices.push_back(float(255));
                    vertices.push_back(float(0));
                    vertices.push_back(float(0));*/
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
    //std::cout << vertices.back();
    /*for (int i = 0; i < sizeof(vertices);)
    {
        std::cout << vertices[i++] << ' ';
        std::cout << vertices[i++] << ' ';
        std::cout << vertices[i++] << ' ';
        std::cout << vertices[i++] << ' ';
        std::cout << vertices[i++] << ' ';
        std::cout << vertices[i++] << '\n';
    }
    std::cout << sizeof(vertices) << '\n';*/

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Сначала связываем объект вершинного массива, затем связываем и устанавливаем вершинный буфер(ы), и затем конфигурируем вершинный атрибут(ы)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer() зарегистрировал VBO как привязанный вершинный буферный объект для вершинного атрибута, так что после этого мы можем спокойно выполнить отвязку
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Вы можете отменить привязку VАО после этого, чтобы другие вызовы VАО случайно не изменили этот VAO (но подобное довольно редко случается).
    // Модификация других VAO требует вызов glBindVertexArray(), поэтому мы обычно не снимаем привязку VAO (или VBO), когда это не требуется напрямую
    glBindVertexArray(0);

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Обработка ввода
        processInput(window);

        // Рендеринг        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Рендеринг треугольника
        ourShader.use();
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_POINTS, 0, 9000);
        glDrawArrays(GL_POINTS, 0, int(vertices.size()/6.));

        // glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода\вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);*/

    // glfw: завершение, освобождение всех ранее задействованных GLFW-ресурсов
    glfwTerminate();
    return 0;
}

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, высота и ширина будут значительно больше, чем указано, на Retina-дисплеях    
    glViewport(0, 0, width, height);
}