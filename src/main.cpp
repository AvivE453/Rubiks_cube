#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Debugger.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>
#include <Camera.h>
#include <RubiksCube.h>
#include <Cube.h>

#include <iostream>

/* Window size */
const unsigned int width = 800;
const unsigned int height = 800;
const float FOVdegree = 45.0f; // Field Of View Angle
const float near = 0.1f;
const float far = 100.0f;

/* Cube vertices coordinates with positions, colors, and corrected texCoords */
float vertices[] = {
    // positions            // colors            // texCoords
    // Back face
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // Top-right
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Top-left

    // Front face
    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // Top-right
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Top-left

    // Left face
    -0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-right
    -0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
    -0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f,  // Top-left
    -0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 1.0f,   // Top-right

    // Right face
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-left
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // Top-left

    // Down face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Top-left
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // Top-right
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // Bottom-right
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom-left

    // Up face
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // Top-left
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // Top-right
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   // Bottom-right
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f   // Bottom-left
};

/* Indices for vertices order */
unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,       // back face
    4, 5, 6, 6, 7, 4,       // front face
    8, 9, 10, 10, 11, 8,    // left face
    12, 13, 14, 14, 15, 12, // right face
    16, 17, 18, 18, 19, 16, // bottom face
    20, 21, 22, 22, 23, 20  // top face
};

int main(int argc, char *argv[])
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Set OpenGL to Version 3.3.0 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Load GLAD so it configures OpenGL */
    gladLoadGL();

    /* Control frame rate */
    glfwSwapInterval(1);

    /* Print OpenGL version after completing initialization */
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    /* Set scope so that on widow close the destructors will be called automatically */
    {
        /* Blend to fix images with transperancy */
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Generate VAO, VBO, EBO and bind them */
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        IndexBuffer ib(indices, sizeof(indices));

        VertexBufferLayout layout;
        layout.Push<float>(3); // positions
        layout.Push<float>(3); // colors
        layout.Push<float>(2); // texCoords
        va.AddBuffer(vb, layout);

        /* Create texture */
        Texture texture("res/textures/plane.png");
        texture.Bind();

        /* Create shaders */
        Shader shader("res/shaders/basic.shader");
        shader.Bind();

        /* Unbind all to prevent accidentally modifying them */
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        /* Enables the Depth Buffer */
        GLCall(glEnable(GL_DEPTH_TEST));

        RubiksCube *rubi = new RubiksCube(3);

        /* Create camera */
        Camera camera(width, height, rubi);
        camera.SetPerspective(FOVdegree, near, far);
        // camera.SetOrthographic(near, far);
        camera.TranslateProjection(glm::vec3(0.0f, 0.0f, -10.0f));
        camera.EnableInputs(window);

        float rotationAngle = 0.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Set white background color */
            GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));

            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            /* Initialize uniform color */
            glm::vec4 color = glm::vec4(1.0, 1.0f, 1.0f, 1.0f);

            /* Initialize the MVP matrices so they are not the null matrix */
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 proj = camera.GetProjectionMatrix();

            /* Initialize the model Translate, Rotate and Scale matrices */
            glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
            glm::mat4 rot = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f));
            glm::mat4 scl = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

            /* Assign different transformations to each matrix */
            rot = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(1.0f, 1.0f, 0.0f));
            model = rot * trans * scl;
            glm::mat4 mvp = proj * view * model;

            /* Update shaders paramters and bind all the necessery objects */
            shader.Bind();
            shader.SetUniform1i("u_PickingMode", 0);
            shader.SetUniform4f("u_Color", color);
            shader.SetUniformMat4f("u_MVP", mvp);
            shader.SetUniform1i("u_Texture", 0);
            va.Bind();
            ib.Bind();

            /* Call the Draw function (EBO is already in Bind state so we send nullptr) */
            GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

            std::vector<std::vector<std::vector<Cube *>>> &cube_indexes = rubi->getCubeIndexes();

            /* Example to draw all the other cubes */

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {

                        trans = cube_indexes[i][j][k]->getTranslationMatrix();
                        rot = cube_indexes[i][j][k]->getRotationMatrix();
                        scl = cube_indexes[i][j][k]->getScaleMatrix();
                        model = rot * trans * scl;
                        mvp = proj * view * model;
                        shader.SetUniformMat4f("u_MVP", mvp);
                        GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
                    }
                }
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            if (camera.m_picking_mode)
            {
                GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
                shader.SetUniform1i("u_PickingMode", 1);
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        for (int k = 0; k < 3; k++)
                        {
                            Cube *c = rubi->cube_at_index(i, j, k);
                            int cubeID = c->id;
                            int r = ((cubeID + 1) & 0x000000FF) >> 0;
                            int g = ((cubeID + 1) & 0x0000FF00) >> 8;
                            int b = ((cubeID + 1) & 0x00FF0000) >> 16;
                            color = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
                            shader.SetUniform4f("u_Color", color);

                            trans = cube_indexes[i][j][k]->getTranslationMatrix();
                            rot = cube_indexes[i][j][k]->getRotationMatrix();
                            scl = cube_indexes[i][j][k]->getScaleMatrix();
                            model = rot * trans * scl;
                            mvp = proj * view * model;
                            shader.SetUniformMat4f("u_MVP", mvp);
                            GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
                        }
                    }
                }
            }

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}