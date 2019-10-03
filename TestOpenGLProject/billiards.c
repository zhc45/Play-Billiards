#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#define _USE_MATH_DEFINES // for C  
#include <math.h>  

#endif  

#include "initShader.h"
#include <time.h>
#include "Library.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int num_vertices;

vec4 cube_vertices[36] =
{ { -1, -1, 1, 1.0}, // front
 {1, -1, 1, 1.0},
 { 1, 1, 1, 1.0},
 { -1, -1, 1, 1.0},
 { 1, 1, 1, 1.0},
 { -1, 1, 1, 1.0},
 { -1, -1, -1, 1.0}, // back
 { 1, 1, -1, 1.0},
 { 1, -1, -1, 1.0},
 { -1, -1, -1, 1.0},
 { -1, 1, -1, 1.0},
 { 1, 1, -1, 1.0},
 { -1, 1, 1, 1.0}, // top
 { 1, 1, 1, 1.0},
 { 1, 1, -1, 1.0},
 { -1, 1, 1, 1.0},
 { 1, 1, -1, 1.0},
 { -1, 1, -1, 1.0},
 { -1, -1, 1, 1.0},	// bottom
 { 1, -1, 1, 1.0},
 { 1, -1, -1, 1.0},
 { -1, -1, 1, 1.0},
 { 1, -1, -1, 1.0},
 { -1, -1, -1, 1.0},
 { -1, -1, 1, 1.0},	// left
 { -1, 1, -1, 1.0},
 { -1, -1, -1, 1.0},
 { -1, -1, 1, 1.0},
 { -1, 1, 1, 1.0},
 { -1, 1, -1, 1.0},
 { 1, -1, 1, 1.0},	// right 
 { 1, -1, -1, 1.0},
 { 1, 1, -1, 1.0},
 { 1, -1, 1, 1.0},
 { 1, 1, -1, 1.0},
 { 1, 1, 1, 1.0}
};

vec4 assign(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	vec4 result = { x, y, z, w };
	return result;
}
float light_x, light_y, light_z;
GLfloat ball_radius = 0.025;
GLfloat x_center = 0.5;
GLfloat z_center = 0.5;
GLuint light_location;
vec4 LightPosition;
vec4 EyePoint;
vec4 shadow_color = {0.0, 0.0, 0.0, 1.0};

GLfloat x_eye; //as x_centerGLfloat y_eye;GLfloat z_eye;

GLfloat at_x = 0.5; //as x_center 
GLfloat at_y = 0.025; //as ball_radius
GLfloat at_z = 0.5; // as z_center


vec4 *generate_vertices(int *num_vertices)
{
	*num_vertices = 36 + (36 * 6 * 18 )*6;
	vec4 *vertices = (vec4*)malloc(sizeof(vec4) * (36 + (36 * 6 * 18) * 6));

	vec4 *sphere_vertices = (vec4*)malloc(sizeof(vec4) * 36 * 6 * 18);
	int theta = 0;
	int alpha = 0;
	int i = 0;

	GLfloat radius = 1;
	GLfloat to_r = M_PI / 180.0;

	while (alpha <= 170) {
		while (theta <= 350) {
			sphere_vertices[i] = assign(radius * sin((alpha)*to_r) * cos((theta)*to_r), radius * sin((theta)*to_r)* sin((alpha)*to_r), radius * cos((alpha)*to_r), 1.0);
			sphere_vertices[i + 1] = assign(radius * sin((alpha + 10)*to_r) * cos((theta)*to_r), radius * sin((theta)*to_r)* sin((alpha + 10)*to_r), radius * cos((alpha + 10)*to_r), 1.0);
			sphere_vertices[i + 2] = assign(radius * sin((alpha + 10)*to_r) * cos((theta + 10)*to_r), radius * sin((theta + 10)*to_r)* sin((alpha + 10)*to_r), radius * cos((alpha + 10)*to_r), 1.0);

			sphere_vertices[i + 3] = assign(radius * sin((alpha)*to_r) * cos((theta)*to_r), radius * sin((theta)*to_r)* sin((alpha)*to_r), radius * cos((alpha)*to_r), 1.0);
			sphere_vertices[i + 4] = assign(radius * sin((alpha + 10)*to_r) * cos((theta + 10)*to_r), radius * sin((theta + 10)*to_r)* sin((alpha + 10)*to_r), radius * cos((alpha + 10)*to_r), 1.0);
			sphere_vertices[i + 5] = assign(radius * sin((alpha)*to_r) * cos((theta + 10)*to_r), radius * sin((theta + 10)*to_r)* sin((alpha)*to_r), radius * cos((alpha)*to_r), 1.0);

			i = i + 6;
			theta = theta + 10;
		}
		alpha = alpha + 10;
		theta = 0;
	}

	int v_index = 0;
	mat4 table_sc = generate_scaling_matrix(0.5, 0.02, 0.5);
	mat4 table_t = generate_translation_matrix(0.5, -0.02, 0.5);
	for (i = 0; i < 36; i++)
	{
		vec4 v1 = matrix_vector_mult(table_sc, cube_vertices[i]);
		vertices[v_index] = matrix_vector_mult(table_t, v1);
		v_index++;
	}

	mat4 b1_sc = generate_scaling_matrix(ball_radius, ball_radius, ball_radius);
	mat4 b1_t = generate_translation_matrix(x_center, ball_radius, z_center);
	for (i = 0; i < 3888; i++)
	{
		vec4 v1 = matrix_vector_mult(b1_sc, sphere_vertices[i]);
		vertices[v_index] = matrix_vector_mult(b1_t, v1);
		v_index++;
	}

	mat4 b2_sc = generate_scaling_matrix(ball_radius, ball_radius, ball_radius);
	mat4 b2_t = generate_translation_matrix(x_center+2*ball_radius, ball_radius, z_center);
	for (i = 0; i < 3888; i++)
	{
		vec4 v1 = matrix_vector_mult(b2_sc, sphere_vertices[i]);
		vertices[v_index] = matrix_vector_mult(b2_t, v1);
		v_index++;
	}

	mat4 b3_sc = generate_scaling_matrix(ball_radius, ball_radius, ball_radius);
	mat4 b3_t = generate_translation_matrix(x_center+4*ball_radius, ball_radius, z_center);
	for (i = 0; i < 3888; i++)
	{
		vec4 v1 = matrix_vector_mult(b3_sc, sphere_vertices[i]);
		vertices[v_index] = matrix_vector_mult(b3_t, v1);
		v_index++;
	}

	mat4 b4_sc = generate_scaling_matrix(ball_radius, ball_radius, ball_radius);
	mat4 b4_t = generate_translation_matrix(x_center+6*ball_radius, ball_radius, z_center);
	for (i = 0; i < 3888; i++)
	{
		vec4 v1 = matrix_vector_mult(b4_sc, sphere_vertices[i]);
		vertices[v_index] = matrix_vector_mult(b4_t, v1);
		v_index++;
	}

	mat4 b5_sc = generate_scaling_matrix(ball_radius, ball_radius, ball_radius);
	mat4 b5_t = generate_translation_matrix(x_center +8*ball_radius, ball_radius, z_center);
	for (i = 0; i < 3888; i++)
	{
		vec4 v1 = matrix_vector_mult(b5_sc, sphere_vertices[i]);
		vertices[v_index] = matrix_vector_mult(b5_t, v1);
		v_index++;
	}

	GLfloat light_radius = 0.003;
	mat4 b6_sc = generate_scaling_matrix(light_radius, light_radius, light_radius);
	mat4 b6_t = generate_translation_matrix(x_center, 0.2, z_center);
	light_x = x_center;
	light_y = 0.2;
	light_z = z_center;

	for (i = 0; i < 3888; i++)
	{
		vec4 v1 = matrix_vector_mult(b6_sc, sphere_vertices[i]);
		vertices[v_index] = matrix_vector_mult(b6_t, v1);
		v_index++;
	}

	return vertices;
}

vec4 *generate_normals(int num_vertices)
{
	vec4 *normals = (vec4*)malloc(sizeof(vec4) * num_vertices);

	vec4 *sphere_colors = (vec4*)malloc(sizeof(vec4) * 36 * 6 * 18);
	int theta = 0;
	int alpha = 0;
	int i = 0;

	GLfloat radius = 1;
	GLfloat to_r = M_PI / 180.0;
	while (alpha <= 170) {
		while (theta <= 350) {
			sphere_colors[i] = assign(radius * sin((alpha)*to_r) * cos((theta)*to_r), radius * sin((theta)*to_r)* sin((alpha)*to_r), radius * cos((alpha)*to_r), 0.0);
			sphere_colors[i + 1] = assign(radius * sin((alpha + 10)*to_r) * cos((theta)*to_r), radius * sin((theta)*to_r)* sin((alpha + 10)*to_r), radius * cos((alpha + 10)*to_r), 0.0);
			sphere_colors[i + 2] = assign(radius * sin((alpha + 10)*to_r) * cos((theta + 10)*to_r), radius * sin((theta + 10)*to_r)* sin((alpha + 10)*to_r), radius * cos((alpha + 10)*to_r), 0.0);

			sphere_colors[i + 3] = assign(radius * sin((alpha)*to_r) * cos((theta)*to_r), radius * sin((theta)*to_r)* sin((alpha)*to_r), radius * cos((alpha)*to_r), 0.0);
			sphere_colors[i + 4] = assign(radius * sin((alpha + 10)*to_r) * cos((theta + 10)*to_r), radius * sin((theta + 10)*to_r)* sin((alpha + 10)*to_r), radius * cos((alpha + 10)*to_r), 0.0);
			sphere_colors[i + 5] = assign(radius * sin((alpha)*to_r) * cos((theta + 10)*to_r), radius * sin((theta + 10)*to_r)* sin((alpha)*to_r), radius * cos((alpha)*to_r), 0.0);

			i = i + 6;
			theta = theta + 10;
		}
		alpha = alpha + 10;
		theta = 0;
	}

	i = 0;
	int v_index = 0;
	for (i = 0; i < 36; i++)
	{
		normals[v_index] = cube_vertices[i];
		v_index++;
	}

	
	for (i = 0; i < 3888; i++)
	{
		normals[v_index] = sphere_colors[i];
		v_index++;
	}

	for (i = 0; i < 3888; i++)
	{
		normals[v_index] = sphere_colors[i];
		v_index++;
	}

	for (i = 0; i < 3888; i++)
	{
		normals[v_index] = sphere_colors[i];
		v_index++;
	}

	for (i = 0; i < 3888; i++)
	{
		normals[v_index] = sphere_colors[i];
		v_index++;
	}

	for (i = 0; i < 3888; i++)
	{
		normals[v_index] = sphere_colors[i];
		v_index++;
	}

	for (i = 0; i < 3888; i++)
	{
		normals[v_index] = assign(1.0, 1.0, 1.0, 1.0);
		v_index++;
	}

	return normals;
}
 
GLuint ctm_location;
mat4 table_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };

int isAnimating = 0;
GLuint isShadow_location;
int isShadow = 0;
GLuint model_view_location;
mat4 model_view_matrix = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
GLuint projection_location;
mat4 projection_matrix = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };

GLuint diffuseProduct_location;
vec4 DiffuseProduct;
GLuint specularProduct_location;
vec4 SpecularProduct;
GLuint ambientProduct_location;
vec4 AmbientProduct;

GLfloat shininess = 10;
GLfloat attenuation_constant = 0;
GLfloat attenuation_linear = 1;
GLfloat attenuation_quadratic = 0;

vec4 light_diffuse = { 1.0, 1.0, 1.0, 1.0 };
vec4 light_specular = { 1.0, 1.0, 1.0, 1.0 };
vec4 light_ambient = { 0.2, 0.2, 0.2, 1.0 };
GLuint ac_location;
GLuint al_location;
GLuint aq_location;
GLuint shininess_location;

mat4 ball_1_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
mat4 ball_2_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
mat4 ball_3_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
mat4 ball_4_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
mat4 ball_5_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
mat4 light_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };

mat4 b1_shadow_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
mat4 b2_shadow_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
mat4 b3_shadow_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
mat4 b4_shadow_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
mat4 b5_shadow_ctm = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };

typedef struct
{
	vec4 reflect_ambient;
	vec4 reflect_diffuse;
	vec4 reflect_specular;
	GLfloat shininess;
} material;

material table_materials = { {0.0, 1, 0.0, 1.0}, {0.0, 0, 0.0, 1.0}, {0.0, 0.0, 0.0, 0.0}, 10 };
material b1_materials = { {1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material b2_materials = { {0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material b3_materials = { {0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material b4_materials = { {1.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material b5_materials = { {1.0, 0.5, 0.0, 1.0}, {1.0, 0.5, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10};
material light_materials = { {0.95, 0.95, 0.95, 1.0}, {0.9, 0.9, 0.9, 1.0}, {0.9, 0.9, 0.9, 1.0}, 10};
GLuint shadow_color_location;
GLuint eye_location;

void init(void)
{
	GLuint program = initShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	int cone_num_vertices;
	vec4 *cone_vertices = generate_vertices(&cone_num_vertices);
	vec4 *cone_normals = generate_normals(cone_num_vertices);
	num_vertices = cone_num_vertices;
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	model_view_matrix = look_at(0.5, 0.2, 1, at_x, at_y, at_z, 0, 1, 0);
	LightPosition = assign(light_x, light_y, light_z, 1.0);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * num_vertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, cone_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, cone_normals);

	print_vector(cone_normals[0]);

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(vec4) * num_vertices));
	
	isShadow_location = glGetUniformLocation(program, "isShadow");
	glUniform1i(isShadow_location, 0);

	ctm_location = glGetUniformLocation(program, "ctm");
	model_view_location = glGetUniformLocation(program, "model_view_matrix");
	projection_location = glGetUniformLocation(program, "projection_matrix");
	
	diffuseProduct_location = glGetUniformLocation(program, "DiffuseProduct");
	ambientProduct_location = glGetUniformLocation(program, "AmbientProduct");
	specularProduct_location = glGetUniformLocation(program, "SpecularProduct");
	
	light_location = glGetUniformLocation(program, "LightPosition");
	shadow_color_location = glGetUniformLocation(program, "shadow_color");

	ac_location = glGetUniformLocation(program, "attenuation_constant");
	al_location = glGetUniformLocation(program, "attenuation_linear");
	aq_location = glGetUniformLocation(program, "attenuation_quadratic");
	shininess_location = glGetUniformLocation(program, "shininess");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDepthRange(1, 0);
}

vec4 product(vec4 a, vec4 b) {
	vec4 result = { a.x*b.x, a.y*b.y, a.z*b.z, 1.0 };
	return result;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glUniform1i(isShadow_location, 0);
	projection_matrix = frustum(-0.1, 0.1, -0.1, 0.1, -0.1, -2);
	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view_matrix);
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection_matrix);
	
	glUniform1fv(ac_location, 1, (GLfloat *)&attenuation_constant);
	glUniform1fv(al_location, 1, (GLfloat *)&attenuation_linear);
	glUniform1fv(aq_location, 1, (GLfloat *)&attenuation_quadratic);
	glUniform1fv(shininess_location, 1, (GLfloat *)&shininess);
	
	
	glUniform4fv(light_location, 1, (GLfloat *)&LightPosition);
	glUniform1i(isShadow_location, 0);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&table_ctm);
	AmbientProduct = product(table_materials.reflect_ambient, light_ambient);
	DiffuseProduct = product(table_materials.reflect_diffuse, light_diffuse);
	SpecularProduct = product(table_materials.reflect_specular, light_specular);
	glUniform4fv(ambientProduct_location, 1, (GLfloat *)&AmbientProduct);
	glUniform4fv(diffuseProduct_location, 1, (GLfloat *)&DiffuseProduct);
	glUniform4fv(specularProduct_location, 1, (GLfloat *)&SpecularProduct);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUniform4fv(light_location, 1, (GLfloat *)&LightPosition);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball_1_ctm);
	AmbientProduct = product(b1_materials.reflect_ambient, light_ambient);
	DiffuseProduct = product(b1_materials.reflect_diffuse, light_diffuse);
	SpecularProduct = product(b1_materials.reflect_specular, light_specular);
	glUniform4fv(ambientProduct_location, 1, (GLfloat *)&AmbientProduct);
	glUniform4fv(diffuseProduct_location, 1, (GLfloat *)&DiffuseProduct);
	glUniform4fv(specularProduct_location, 1, (GLfloat *)&SpecularProduct);
	glDrawArrays(GL_TRIANGLES, 36, 3888);

	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball_2_ctm);
	AmbientProduct = product(b2_materials.reflect_ambient, light_ambient);
	DiffuseProduct = product(b2_materials.reflect_diffuse, light_diffuse);
	SpecularProduct = product(b2_materials.reflect_specular, light_specular);
	glUniform4fv(ambientProduct_location, 1, (GLfloat *)&AmbientProduct);
	glUniform4fv(diffuseProduct_location, 1, (GLfloat *)&DiffuseProduct);
	glUniform4fv(specularProduct_location, 1, (GLfloat *)&SpecularProduct);
	glDrawArrays(GL_TRIANGLES, 3888+36, 3888);

	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball_3_ctm);
	AmbientProduct = product(b3_materials.reflect_ambient, light_ambient);
	DiffuseProduct = product(b3_materials.reflect_diffuse, light_diffuse);
	SpecularProduct = product(b3_materials.reflect_specular, light_specular);
	glUniform4fv(ambientProduct_location, 1, (GLfloat *)&AmbientProduct);
	glUniform4fv(diffuseProduct_location, 1, (GLfloat *)&DiffuseProduct);
	glUniform4fv(specularProduct_location, 1, (GLfloat *)&SpecularProduct);
	glDrawArrays(GL_TRIANGLES, 36 + (3888 * 2), 3888);

	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball_4_ctm);
	AmbientProduct = product(b4_materials.reflect_ambient, light_ambient);
	DiffuseProduct = product(b4_materials.reflect_diffuse, light_diffuse);
	SpecularProduct = product(b4_materials.reflect_specular, light_specular);
	glUniform4fv(ambientProduct_location, 1, (GLfloat *)&AmbientProduct);
	glUniform4fv(diffuseProduct_location, 1, (GLfloat *)&DiffuseProduct);
	glUniform4fv(specularProduct_location, 1, (GLfloat *)&SpecularProduct);
	glDrawArrays(GL_TRIANGLES, 36 + (3888 * 3), 3888);

	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ball_5_ctm);
	AmbientProduct = product(b5_materials.reflect_ambient, light_ambient);
	DiffuseProduct = product(b5_materials.reflect_diffuse, light_diffuse);
	SpecularProduct = product(b5_materials.reflect_specular, light_specular);
	glUniform4fv(ambientProduct_location, 1, (GLfloat *)&AmbientProduct);
	glUniform4fv(diffuseProduct_location, 1, (GLfloat *)&DiffuseProduct);
	glUniform4fv(specularProduct_location, 1, (GLfloat *)&SpecularProduct);
	glDrawArrays(GL_TRIANGLES, 36 + (3888 * 4), 3888);

	glUniform1i(isShadow_location, 1);
	glUniform4fv(shadow_color_location, 1, (GLfloat *)&shadow_color);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&b1_shadow_ctm); // b1 shadow
	glDrawArrays(GL_TRIANGLES, 36, 3888);

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&b2_shadow_ctm); //b2 shadow
	glDrawArrays(GL_TRIANGLES, 36 + (3888), 3888);

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&b3_shadow_ctm); //b3 shadow
	glDrawArrays(GL_TRIANGLES, 36 + (3888 * 2), 3888);

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&b4_shadow_ctm); //b4 shadow
	glDrawArrays(GL_TRIANGLES, 36 + (3888 * 3), 3888);

	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&b5_shadow_ctm); //b5 shadow
	glDrawArrays(GL_TRIANGLES, 36 + (3888 * 4), 3888);

	glUniform1i(isShadow_location, 0);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&light_ctm);
	AmbientProduct = light_materials.reflect_ambient;
	DiffuseProduct = light_materials.reflect_ambient;
	SpecularProduct = light_materials.reflect_ambient;
	glUniform4fv(ambientProduct_location, 1, (GLfloat *)&AmbientProduct);
	glUniform4fv(diffuseProduct_location, 1, (GLfloat *)&DiffuseProduct);
	glUniform4fv(specularProduct_location, 1, (GLfloat *)&SpecularProduct);
	glDrawArrays(GL_TRIANGLES, 36 + (3888 * 5), 3888);

	glutSwapBuffers();
}

GLfloat r = 0.5;
GLfloat x_eye = 0.5;
GLfloat y_eye = 0.2;
GLfloat y_center = 0.2;GLfloat z_eye = 1;

GLfloat theta_x = 0;
GLfloat theta_y = 0;
GLfloat theta_z = 0;

GLfloat to_r = M_PI / 180.0;

void keyboard(unsigned char key, int x, int y)
{
	
	if (key == 'q') {
		exit(0);
	}
	else if (key == 'e') { //light source up
		mat4 m1_b1 = matrix_matrix_mult(generate_translation_matrix(0, 0.003, 0), generate_translation_matrix(-light_x, -light_y, -light_z));
		mat4 m2_b1 = matrix_matrix_mult(generate_translation_matrix(light_x, light_y, light_z), m1_b1);
		light_ctm = matrix_matrix_mult(m2_b1, light_ctm);
		light_y = light_y + 0.003;
		LightPosition = assign(light_x, light_y, light_z, 1.0);
	}
	else if (key == 'd') { //light source down
		mat4 m1_b1 = matrix_matrix_mult(generate_translation_matrix(0, -0.003, 0), generate_translation_matrix(-light_x, -light_y, -light_z));
		mat4 m2_b1 = matrix_matrix_mult(generate_translation_matrix(light_x, light_y, light_z), m1_b1);
		light_ctm = matrix_matrix_mult(m2_b1, light_ctm);
		light_y = light_y - 0.003;
		LightPosition = assign(light_x, light_y, light_z, 1.0);
	}
	else if (key == 's') { //light source left
		mat4 m1_b1 = matrix_matrix_mult(generate_translation_matrix(-0.003, 0, 0), generate_translation_matrix(-light_x, -light_y, -light_z));
		mat4 m2_b1 = matrix_matrix_mult(generate_translation_matrix(light_x, light_y, light_z), m1_b1);
		light_ctm = matrix_matrix_mult(m2_b1, light_ctm);
		light_x = light_x - 0.003;
		LightPosition = assign(light_x, light_y, light_z, 1.0);
	}
	else if (key == 'f') { //light source right
		mat4 m1_b1 = matrix_matrix_mult(generate_translation_matrix(0.003, 0, 0), generate_translation_matrix(-light_x, -light_y, -light_z));
		mat4 m2_b1 = matrix_matrix_mult(generate_translation_matrix(light_x, light_y, light_z), m1_b1);
		light_ctm = matrix_matrix_mult(m2_b1, light_ctm);
		light_x = light_x + 0.003;
		LightPosition = assign(light_x, light_y, light_z, 1.0);
	}
	else if (key == 'x') { // light source forward
		mat4 m1_b1 = matrix_matrix_mult(generate_translation_matrix(0, 0, -0.003), generate_translation_matrix(-light_x, -light_y, -light_z));
		mat4 m2_b1 = matrix_matrix_mult(generate_translation_matrix(light_x, light_y, light_z), m1_b1);
		light_ctm = matrix_matrix_mult(m2_b1, light_ctm);
		light_z = light_z - 0.003;
		LightPosition = assign(light_x, light_y, light_z, 1.0);
	}
	else if (key == 'c') { // light source backward
		mat4 m1_b1 = matrix_matrix_mult(generate_translation_matrix(0, 0, 0.003), generate_translation_matrix(-light_x, -light_y, -light_z));
		mat4 m2_b1 = matrix_matrix_mult(generate_translation_matrix(light_x, light_y, light_z), m1_b1);
		light_ctm = matrix_matrix_mult(m2_b1, light_ctm);
		light_z = light_z + 0.003;
		LightPosition = assign(light_x, light_y, light_z, 1.0);
	}
	else if (key == 'i') { //viewer up
		theta_y += 0.01;
		theta_z += 0.01;
		y_eye = y_center + r * sin(theta_y);
		z_eye = z_center + r * cos(theta_z);	
		model_view_matrix = look_at(x_eye, y_eye, z_eye, at_x, at_y, at_z, 0, 1, 0);
	}
	else if (key == 'k') { //viewer down
		theta_y -= 0.01;
		theta_z -= 0.01;
		y_eye = y_center + r * sin(theta_y);
		z_eye = z_center + r * cos(theta_z);
		model_view_matrix = look_at(x_eye, y_eye, z_eye, at_x, at_y, at_z, 0, 1, 0);
	}
	else if (key == 'j') { //viewer left
		theta_x -= 0.01;
		theta_z -= 0.01;
		x_eye = x_center + r * sin(theta_x);
		z_eye = z_center + r * cos(theta_z);
		model_view_matrix = look_at(x_eye, y_eye, z_eye, at_x, at_y, at_z, 0, 1, 0);
	}
	else if (key == 'l') { //viewer right
		theta_x += 0.01;
		theta_z += 0.01;
		x_eye = x_center + r * sin(theta_x);
		z_eye = z_center + r * cos(theta_z);
		model_view_matrix = look_at(x_eye, y_eye, z_eye, at_x, at_y, at_z, 0, 1, 0);
	}
	else if (key == 'n') { // viewer closer
		r = r - 0.01;
		x_eye = x_center + r * sin(theta_x);
		y_eye = y_center + r * sin(theta_y);
		z_eye = z_center + r * cos(theta_z);
		model_view_matrix = look_at(x_eye, y_eye, z_eye, at_x, at_y, at_z, 0, 1, 0);
	}
	else if (key == 'm') { //viewer further
		r = r + 0.01;
		x_eye = x_center + r * sin(theta_x);
		y_eye = y_center + r * sin(theta_y);
		z_eye = z_center + r * cos(theta_z);
		model_view_matrix = look_at(x_eye, y_eye, z_eye, at_x, at_y, at_z, 0, 1, 0);
	}
	else if (key == '1') {
		isAnimating = 1;
	}
	else if (key == '0') {
		isAnimating = 0;
	}
			
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	glViewport(0, 0, 512, 512);
}

GLfloat angle_b2 = 0;
GLfloat angle_b3 = 0;
GLfloat angle_b4 = 0;
GLfloat angle_b5 = 0;

void idle(void) {

	if (isAnimating)
	{
		angle_b2 += 0.003;
		if (fabs(angle_b2 - 2 * M_PI) < 0.003) {
			angle_b2 = 0;
		}
		else {
			ball_2_ctm = generate_translation_matrix(2 * ball_radius * (cos(angle_b2) - 1), 0, 2 * ball_radius * (sin(angle_b2)));
			b2_shadow_ctm = generate_translation_matrix(2 * ball_radius * (cos(angle_b2) - 1), 0, 2 * ball_radius * (sin(angle_b2)));
		}

		angle_b3 += 0.002;
		if (fabs(angle_b3 - 2 * M_PI) < 0.002) {
			angle_b3 = 0;
		}
		else {
			ball_3_ctm = generate_translation_matrix(4 * ball_radius * (cos(angle_b3) - 1), 0, 4 * ball_radius * (-sin(angle_b3)));
			b3_shadow_ctm = generate_translation_matrix(4 * ball_radius * (cos(angle_b3) - 1), 0, 4 * ball_radius * (-sin(angle_b3)));
		}

		angle_b4 += 0.001;
		if (fabs(angle_b4 - 2 * M_PI) < 0.001) {
			angle_b4 = 0;
		}
		else {
			ball_4_ctm = generate_translation_matrix(6 * ball_radius * (cos(angle_b4) - 1), 0, 6 * ball_radius * (sin(angle_b4)));
			b4_shadow_ctm = generate_translation_matrix(6 * ball_radius * (cos(angle_b4) - 1), 0, 6 * ball_radius * (sin(angle_b4)));
		}

		angle_b5 += 0.0015;
		if (fabs(angle_b5 - 2 * M_PI) < 0.0015) {
			angle_b5 = 0;
		}
		else {
			ball_5_ctm = generate_translation_matrix(8 * ball_radius * (cos(angle_b5) - 1), 0, 8 * ball_radius * (sin(angle_b5)));
			b5_shadow_ctm = generate_translation_matrix(8 * ball_radius * (cos(angle_b5) - 1), 0, 8 * ball_radius * (sin(angle_b5)));
		}
	}
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Table&Balls");
	glewInit();
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	
	glutMainLoop();
	return 0;
}

