#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.inl>
#include "GLFWCallbacks.h"
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Learning Graphics Programming", NULL, NULL);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, GLFWCallbacks::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return -1;
    }

    //stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    
    Shader phongShader("Assets/Shaders/Vertex/colors.vert", "Assets/Shaders/Fragment/TestFragmentShader.frag");
    Model ourModel((char*)"Assets/Models/the_bathroom_free/scene.gltf");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        phongShader.use();
        phongShader.setVec3("viewPos", camera.Position);
        phongShader.setFloat("material.shininess", 32.0f);

        phongShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        phongShader.setVec3("dirLight.ambient", 0.35f, 0.35f, 0.35f);
        phongShader.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
        phongShader.setVec3("dirLight.specular", 0.7f, 0.7f, 0.7f);

        phongShader.setVec3("pointLights[0].position", glm::vec3(1.0f, 2.0f, 3.0f));
        phongShader.setVec3("pointLights[0].ambient", 0.00f, 0.00f, 0.05f);
        phongShader.setVec3("pointLights[0].diffuse", 0.0f, 0.0f, 0.8f);
        phongShader.setVec3("pointLights[0].specular", 0.0f, 0.0f, 1.0f);
        phongShader.setFloat("pointLights[0].constant", 1.0f);
        phongShader.setFloat("pointLights[0].linear", 0.09f);
        phongShader.setFloat("pointLights[0].quadratic", 0.032f);

        phongShader.setVec3("pointLights[1].position", glm::vec3(1.0f, 2.0f, 0.0f));
        phongShader.setVec3("pointLights[1].ambient", 0.05f, 0.00f, 0.00f);
        phongShader.setVec3("pointLights[1].diffuse", 0.8f, 0.0f, 0.0f);
        phongShader.setVec3("pointLights[1].specular", 1.0f, 0.0f, 0.0f);
        phongShader.setFloat("pointLights[1].constant", 1.0f);
        phongShader.setFloat("pointLights[1].linear", 0.09f);
        phongShader.setFloat("pointLights[1].quadratic", 0.032f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        phongShader.setMat4("projection", projection);
        phongShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        phongShader.setMat4("model", model);
        ourModel.Draw(phongShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}