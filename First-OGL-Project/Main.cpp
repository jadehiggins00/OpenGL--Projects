#define _CRT_SECURE_NO_WARNINGS
#include <glew.h>    /* include GLEW and new version of GL on Windows */
#include <glfw3.h> /* GLFW helper library */

#include <stdio.h>
#include <stdarg.h> // for doing gl_log() functions that work like printf()
#include <time.h>

#define GL_LOG_FILE "gl.log"
#include "shaders.h"


/*
This function opends the log file and prints the date/time 
*/
bool restart_gl_log() {

    FILE* file = fopen(GL_LOG_FILE, "w");
    if (!file) {
        fprintf(
            stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
            GL_LOG_FILE
        );
        return false;
    }// end if
    time_t now = time(NULL);
    char* date = ctime(&now);
    fprintf(file, "GL_LOG_FILE LOG. local time %s\n", date);
    fclose(file);
    return true;

}//end func

/*
this is the main log print-out
the "..." is a part of C's variables format and lets us give any number of parameters.
*/
bool gl_log(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if (!file) {
        fprintf(
            stderr,"ERROR: could not open GL_LOG_FILE %s file for appending\n",
            GL_LOG_FILE
        );
        return false;
    }//end if
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}// end function

/*
This is a GL log funciton for Error messages
*/
bool gl_log_err(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if (!file) {
        fprintf(
            stderr,
            "ERROR: could not open GL_LOG_FILE %s file for appending\n",
            GL_LOG_FILE
        );
        return false;
    }//end if
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);

    fclose(file);
    return true;
}// end function





int main() {
    GLFWwindow* window = NULL;
  //  const GLubyte* renderer;
  //  const GLubyte* version;

    /* start GL context and O/S window using the GLFW helper library */
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    // creating the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // points for triangle
    GLfloat points[] = {
        -0.5f, 0.0f, 0.0f,
        -0.5f, -0.8f, 0.0f,
        0.5f, -0.8f, 0.0f
    };

    // points for second triangle
    GLfloat points2[] = {

        -0.5f, 0.0f, 0.0f,
        0.5f, -0.8f, 0.0f,
        0.5f, 0.0f, 0.0f
    };

    // we will now copy this chunk of memory onto the graphics card using the vertex buffer unit
    // to do this we generate an empty buffer
    GLuint vbo;
    glGenBuffers(1, &vbo);
    // we will then set the current buffer in Opengl by binding then copy the points into the currently bound buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // we then tell opengl that the buffer is of size 9 floating point numbers and we give it the address of the first value
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

    //second VBO-2
    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);

    // we use a vertex attribute object (vao) which is used to remember all the 
    // vertex buffers that you want to use and the memoryy layout of each one.
    // we set up the VAO object once per mesh. when we want to draw, all we do is bind the VAO and draw
    GLuint vao;
    // here we generate a VAO -  we're only using a single VBO so the attribute location will be 0
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
   // we bind it together
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // this defines the layout of our first VBO 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // second vao object 
    GLuint vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    
    // before using shaders we have to load the strings into a GL shader and compile them
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    // we create an empty GPU shader programme, attach the shaders and then link them together
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    //  ------------- creating another shader programme ----------- 
    GLuint vs2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs2, 1, &vertex_shader, NULL);
    glCompileShader(vs2);
    GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs2, 1, &fragment_shader2, NULL);
    glCompileShader(fs2);
    GLuint shader_programme2 = glCreateProgram();
    glAttachShader(shader_programme2, fs2);
    glAttachShader(shader_programme2, vs2);
    glLinkProgram(shader_programme2);


    // changing the background color
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

    // using a loop to draw!
    while (!glfwWindowShouldClose(window)) {


        // clear the drawing surface
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme); // we set the shader program that should be used for all drawing
        // we set the VAO as the input variables that should be used for all further drawings
      
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shader_programme2);
        glBindVertexArray(vao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        
       
        //glfw3 requires that we update events like input handling - like keypresses
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);

    }//end while

      // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}