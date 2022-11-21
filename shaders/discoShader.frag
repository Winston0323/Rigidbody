#version 330 core
in vec3 phongOutput;
in vec3 normalOutput;
in vec3 posOutput;

// uniforms used for lighting
uniform vec3 AmbientColor = vec3(0.2);
uniform vec3 LightDirection = normalize(vec3(0, 20, 10));
uniform vec3 LightColor = vec3(1);
uniform vec3 DiffuseColor;	// passed in from c++ side NOTE: you can also set the value here and then remove 
							// color from the c++ side

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;


void main()
{             
    // ambient
    vec3 ambient = 1 * AmbientColor;
  	
    // diffuse 
    vec3 norm = normalize(phongOutput);
    //getting light vector
    //Li*n
    float diff = max(dot(phongOutput, LightDirection), 0.0);
        
    //ci * (Li*n*kd)
    vec3 diffuse = intense * (diff * material.diffuse);
    
    // specular
    //eye vector
    vec3 viewDir = normalize(viewPos - posOutput);
    //reflection vector
    vec3 reflectDir = reflect(-lightDir, phongOutput);  
    //(eyevector * reflection)^shininess
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //ci*ks*spec
    vec3 specular = intense * (spec * material.specular);  
        
    vec3 result = ambient +diffuse + specular;
    fragColor = vec4(result, 1.0);
}