#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 ourColor; // 向片段着色器输出一个颜色

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  ourColor = vec3(1.0, 0.7, 0.5);
}
