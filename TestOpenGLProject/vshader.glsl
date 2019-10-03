#version 120

attribute vec4 vPosition;
attribute vec4 vNormal;

varying vec4 color;

uniform int isShadow;
uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform mat4 ctm;

uniform vec4 LightPosition;
uniform vec4 shadow_color;
uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct;
uniform vec4 SpecularProduct;
uniform vec4 EyePoint;

uniform float shininess;
uniform float attenuation_constant;
uniform float attenuation_linear;
uniform float attenuation_quadratic;

vec4 ambient, diffuse, specular;



void main()
{
	if(isShadow == 1)
	{
		color = shadow_color;
		vec4 temp_p = ctm * vPosition;
		float x = LightPosition.x - LightPosition.y * (LightPosition.x - temp_p.x) / (LightPosition.y - temp_p.y);
		float y = 0.0001;
		float z = LightPosition.z - LightPosition.y * (LightPosition.z - temp_p.z) / (LightPosition.y - temp_p.y);
		gl_Position =  projection_matrix * model_view_matrix * vec4(x, y, z, 1.0);
		
	}else{

		ambient = AmbientProduct;
		vec4 temp_p = ctm * vPosition;

		vec4 N = normalize(model_view_matrix * vNormal);
		vec4 L_temp = normalize(model_view_matrix * (LightPosition - temp_p));
		vec4 L = L_temp;
		diffuse = max(dot(L,N), 0.0) * DiffuseProduct;

		vec4 V = normalize(vec4(0.0, 0.0, 0.0, 1.0) - (model_view_matrix * temp_p));
		//vec4 R = normalize(2 * dot(L, N) * N - L);
		//specular = pow(max(dot(V, R), 0.0), shininess) * SpecularProduct;
		vec4 H = normalize(L + V);
		specular = pow(max(dot(N, H), 0.0), shininess) * SpecularProduct;

		float distance = length(L_temp);
		float attenuation = 1/(attenuation_constant + (attenuation_linear * distance) + (attenuation_quadratic * distance * distance));

		color = ambient + attenuation * (diffuse + specular);
		//color = specular;
		//color = N;

		gl_Position =  projection_matrix * model_view_matrix * ctm * vPosition;
	}
}
