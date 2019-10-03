#ifndef LIBRARY_H_
#define LIBRARY_H_

#ifdef __APPLE__  // include Mac OS X verions of headers
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif  // __APPLE__

typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
} vec4;

typedef struct
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
} vec3;

typedef struct
{
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 w;
} mat4;

typedef struct
{
	vec3 x;
	vec3 y;
	vec3 z;
} mat3;

void print_vector(vec4 v);
vec4 vector_scalar_mult(GLfloat s, vec4 v);
vec4 vector_addition(vec4 v1, vec4 v2);
vec4 vector_subtraction(vec4 v1, vec4 v2);
GLfloat dot_product(vec4 v1, vec4 v2);
vec4 cross_product(vec4 v1, vec4 v2);

void print_matrix(mat4 m);
mat4 matrix_scalar_mult(GLfloat s, mat4 m);
mat4 matrix_addition(mat4 m1, mat4 m2);
mat4 matrix_subtraction(mat4 m1, mat4 m2);
mat4 matrix_transpose(mat4 m);
mat4 matrix_matrix_mult(mat4 m1, mat4 m2);
vec4 matrix_vector_mult(mat4 m, vec4 v);
mat4 matrix_inverse(mat4 m);
mat4 generate_translation_matrix(GLfloat x, GLfloat y, GLfloat z);
mat4 generate_scaling_matrix(GLfloat x, GLfloat y, GLfloat z);
mat4 rotate_about_x(GLfloat theta);
mat4 rotate_about_y(GLfloat theta);
mat4 rotate_about_z(GLfloat theta);

mat4 look_at(GLfloat eyex, GLfloat eyey, GLfloat eyez,
	GLfloat atx, GLfloat aty, GLfloat atz,
	GLfloat upx, GLfloat upy, GLfloat upz);mat4 frustum(GLfloat left, GLfloat right,
	GLfloat bottom, GLfloat top,
	GLfloat near, GLfloat far);

#endif