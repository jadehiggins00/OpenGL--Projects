#pragma once
// we use GLSL to define how to draw our shape from the VAO
 // the shader program is made up of 2 parts: vertex shader - where the 3d points should
 // end up on the display and a frament shader which colours the surface.
const char* vertex_shader =
"#version 120\n" // version of Opengl
"attribute vec3 vp;"    //has one input vvarr - vec3(vector made up of 3 floats) which matches with VAO attribute pointer
"void main () {"
" gl_Position = vec4 (vp.x,vp.y, vp.z, 1.0);"    // the 1.0 - means dont calculate any perspective
"}";

// fragment shader
const char* fragment_shader =
"#version 120\n"
"void main () {"
" gl_FragColor = vec4 (0.5, 1.0, 0.9, 1.0);"
"}";

