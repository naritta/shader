#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include "shader.hpp"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace std;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    // build and compile our shader program
    // ------------------------------------
    ////    Shader ourShader("shader.vert", "shader.frag"); // you can name your shader files however you like
    //    Shader ourShader("/Users/ritta/Desktop/shader/game/game/shader.vert", "/Users/ritta/Desktop/shader/game/game/shader.frag"); // you can name your shader files however you like
    // Create and compile our GLSL program from the shaders
    GLuint shaderID = LoadShaders("/Users/ritta/Desktop/shader/game/game/raymarch.vert", "/Users/ritta/Desktop/shader/game/game/raymarch.frag");
    //    GLuint shaderID = LoadShaders("shader.vert", "shader.frag");
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //    float vertices[] = {
    //        // positions         // colors
    //        1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    //        -1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    //        1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // top
    //    };
    float positions_vertices[] = {
        // positions
        -1.0f, -1.0f, 0.5f,
        1.0f, -1.0f, 0.5f,
        -1.0f,  1.0f, 0.5f,
        1.0f,  1.0f, 0.5f,
    };
    
    unsigned int positions_VBO, colors_VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &positions_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, positions_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions_vertices), positions_vertices, GL_STREAM_DRAW);
    
    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, positions_VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
    
    // variables for timing
    double prevTime = glfwGetTime();
    double currTime = 0;
    double deltaTime = 0;
    unsigned int frames = 0;
    double fpsInterval = 0;
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // update delta time
        currTime = glfwGetTime();
        deltaTime = currTime - prevTime;
        prevTime = currTime;
        //        cout << currTime << endl;
        
        //        vertices[2]-=deltaTime*0.1;
        //        vertices[8]-=deltaTime*0.1;
        //        vertices[14]-=deltaTime*0.1;
        
        //        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // position attribute
        //        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        //        glEnableVertexAttribArray(0);
        
        // input
        // -----
        processInput(window);
        
        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // render the triangle
        glUseProgram(shaderID);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glUniform2f(glGetUniformLocation(shaderID, "resolution"), SCR_WIDTH, SCR_WIDTH);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &positions_VBO);
    glDeleteProgram(shaderID);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
