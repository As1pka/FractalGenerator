#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ctime>
#include <chrono>

#include "shader_s.h"
#include "VerticesCreator.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ���������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: ������������� � ����������������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // ���������������� ��� ������, ���� ����������� macOS
#endif

    // glfw: �������� ����
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: �������� ���� ���������� �� OpenGL-�������
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // �������������� ����� ��������� ���������
    Shader ourShader("shader.vs", "shader.fs");  // ���� � ������ ��������

    //�������� ������ (� ������(��)) � ��������� ��������� ���������
    //float vertices[] = {
    //    // ����������       // �����
    //    0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // ������ ������ �������
    //   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // ������ ����� �������
    //    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // ������� �������
    //}

    // ���������� ������
    std::vector<float> vertices;
    VerticesCreator v_creator;
    v_creator.setBorder(SCR_WIDTH, SCR_HEIGHT);
    auto begin = std::chrono::steady_clock::now();   
    
    //v_creator.setType(vert_type::levi2D);
    //v_creator.setType(vert_type::levi3D);
    //v_creator.setType(vert_type::mandelbrot);
    v_creator.setType(vert_type::mandelbrot3D);
    //v_creator.setType(vert_type::mandelbrot_wo_bg);
    //v_creator.setType(vert_type::mandelbrot_parallel);
    //v_creator.setType(vert_type::mandelbrot_parallel_wo_bg);
    //v_creator.setType(vert_type::mandelbrot_parallel_wo_bg_half_scene);
    //v_creator.setType(vert_type::triangle);
    if (v_creator.getType() == vert_type::levi2D)
        v_creator.setLeviN(9);
    else if (v_creator.getType() == vert_type::levi3D)
    {
        v_creator.setLeviN(9);
        v_creator.setLeviDiv(10);
    }
    else if (v_creator.getType() == vert_type::mandelbrot3D)
    {
        v_creator.setMandN(2);
        v_creator.setMandDiv(17);
    }
    else
    {
        v_creator.setMandN(2);
    }

    v_creator.getVertices(vertices);

    if (v_creator.getType() == vert_type::levi3D || v_creator.getType() == vert_type::mandelbrot3D)
    {
        // ������� � .obj
        std::ofstream fout("mndb.obj");
        fout << "o obj_0\n";
        for (int i = 0, f = 1; i < vertices.size(); i += 6, f++)
        {
            fout << "v ";
            int j = 0;
            for (j; j < 3; j++)
                fout << vertices[i + j] << " ";
            fout << "\n";
        }
        for (int i = 1; i <= vertices.size() / 6.f; i += 3)
        {
            fout << "f ";
            for (int k = 0; k < 3; k++)
                fout << i + k << " ";
            fout << "\n";
        }
        fout.close();
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // ������� ��������� ������ ���������� �������, ����� ��������� � ������������� ��������� �����(�), � ����� ������������� ��������� �������(�)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // �������� ��������, ��� ������ �������� ���������, ����� glVertexAttribPointer() ��������������� VBO ��� ����������� ��������� �������� ������ ��� ���������� ��������, ��� ��� ����� ����� �� ����� �������� ��������� �������
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // �� ������ �������� �������� V�� ����� �����, ����� ������ ������ V�� �������� �� �������� ���� VAO (�� �������� �������� ����� ���������).
    // ����������� ������ VAO ������� ����� glBindVertexArray(), ������� �� ������ �� ������� �������� VAO (��� VBO), ����� ��� �� ��������� ��������
    glBindVertexArray(0);

    // ���� ����������
    while (!glfwWindowShouldClose(window))
    {
        // ��������� �����
        processInput(window);

        // ���������        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ��������� ������������
        ourShader.use();
        glBindVertexArray(VAO);

        if (v_creator.getType() == vert_type::triangle)
            glDrawArrays(GL_TRIANGLES, 0, 3);
        else if (v_creator.getType() == vert_type::levi2D)
            glDrawArrays(GL_LINES, 0, int(vertices.size() / 6.));
        else if (v_creator.getType() == vert_type::levi3D)
            glDrawArrays(GL_TRIANGLES, 0, int(vertices.size() / 6.));
        else
            glDrawArrays(GL_POINTS, 0, int(vertices.size()/6.));
        
        

        // glfw: ����� ���������� front- � back- �������. ������������ ������� �����\������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
        glfwSwapBuffers(window);
        glfwPollEvents();


        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        std::cout << "The time: " << elapsed_ms.count() << " ms\n";
        //return 0;
    }

    // �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);*/

    // glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
    glfwTerminate();
    return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ ���� � ������ ����� � ��������������� ��������� ������ �������
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ����������, ��� ���� ��������� ������������� ����� �������� ����.
    // �������� ��������, ������ � ������ ����� ����������� ������, ��� �������, �� Retina-��������    
    glViewport(0, 0, width, height);
}