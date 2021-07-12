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
    GLuint shaderID = LoadShaders("/Users/ritta/Desktop/shader/game/game/shader.vert", "/Users/ritta/Desktop/shader/game/game/shader.frag");
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
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    
    float colors_vertices[] = {
        // colors
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f,  0.0f, 2.0f,
    };
    
    unsigned int positions_VBO, colors_VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &positions_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, positions_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions_vertices), positions_vertices, GL_STREAM_DRAW);
    
    glGenBuffers(1, &colors_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors_vertices), colors_vertices, GL_STREAM_DRAW);
    
    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, positions_VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // color attribute
    glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
    
    // Projection matrix : 90° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
                                       glm::vec3(0,0,-2), // Camera is at (0,0,0), in World Space
                                       glm::vec3(0,0,0), // and looks at the origin
                                       glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                       );
    
//    glm::mat4 Model = myTranslationMatrix * myRotationMatrix * myScaleMatrix;
    glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, 0.0f, glm::vec3(1.0f));
    Model = glm::scale(Model, glm::vec3(1.0f));
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
    
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
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
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
    glDeleteBuffers(1, &colors_VBO);
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
