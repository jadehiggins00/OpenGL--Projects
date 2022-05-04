
#include <glew.h>    /* include GLEW and new version of GL on Windows */
#include <glfw3.h> /* GLFW helper library */
#include <stdio.h>

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
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
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

    // we use GLSL to define how to draw our shape from the VAO
    // the shader program is made up of 2 parts: vertex shader - where the 3d points should
    // end up on the display and a frament shader which colours the surface.
    const char* vertex_shader =
        "#version 120\n" // version of Opengl
        "attribute vec3 vp;"    //has one input vvarr - vec3(vector made up of 3 floats) which matches with VAO attribute pointer
        "void main () {"
        " gl_Position = vec4 (vp, 1.0);"    // the 1.0 - means dont calculate any perspective
        "}";

    // fragment shader
    const char* fragment_shader = 
        "#version 120\n"
        "void main () {"
        " gl_FragColor = vec4 (0.5, 1.0, 0.9, 1.0);"
        "}";
    
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

    // using a loop to draw!
    while (!glfwWindowShouldClose(window)) {

        // clear the drawing surface
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme); // we set the shader program that should be used for all drawing
        // we set the VAO as the input variables that should be used for all further drawings
        glBindVertexArray(vao);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glfw3 requires that we update events like input handling - like keypresses
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);

    }//end while

      // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}