#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 normOutput;
in vec3 posOutput;
uniform vec3 lightDir = normalize(vec3(0, -10 , 0));
uniform vec4 astColor;
uniform vec3 viewPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
	float intensity;
	vec4 color;
	vec3 L = normalize(-lightDir);
	
	float diffuse = max(dot(normOutput , L),0); 
	
	vec3 viewDir = normalize(viewPos - posOutput);
    vec3 norm = normalize(normOutput);
	//reflection vector
    vec3 reflectDir = reflect(-lightDir, norm);  
    //(eyevector * reflection)^shininess
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.2f);

	intensity = diffuse;

	if (intensity > 0.95)
		color = vec4(1.0,1.0,1.0,1.0) * astColor;
	else if (intensity > 0.5)
		color = vec4(0.7,0.7,0.7,1.0) * astColor;
	else if (intensity > 0.25)
		color = vec4(0.35,0.35,0.35,1.0) * astColor;
	else
		color = vec4(0.1,0.1,0.1,1.0) * astColor;
	//draw edge
	float edge  = max(0,dot(norm, viewDir));
	if (edge < 0.3){
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	fragColor = color;

}