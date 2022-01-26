#version 450  

layout( location = 0) out vec4 FragColor;  
uniform float BrightnessU ; 
in vec3 Color ;     

void main() 
{
   
    FragColor = vec4(Color, 1.0f);

}    

