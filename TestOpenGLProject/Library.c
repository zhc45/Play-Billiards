#pragma
#include "Library.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void print_vector(vec4 v)
{
	printf("{%f, %f, %f, %f}\n ", v.x, v.y, v.z, v.w);
}

vec4 vector_scalar_mult(GLfloat s, vec4 v)
{
	vec4 result = { s*v.x, s*v.y, s*v.z, s*v.w };
	return result;
}

vec4 vector_addition(vec4 v1, vec4 v2)
{
	vec4 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
	return result;
}

vec4 vector_subtraction(vec4 v1, vec4 v2)
{
	vec4 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
	return result;
}

GLfloat dot_product(vec4 v1, vec4 v2)
{
	GLfloat result = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
	//printf("Dot product %.2f", result);
	return result;
}

vec4 cross_product(vec4 v1, vec4 v2)
{
	vec4 result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x, 0 };
	return result;
}

void print_matrix(mat4 m)
{
	print_vector(m.x);
	print_vector(m.y);
	print_vector(m.z);
	print_vector(m.w);
	printf("\n");
}

mat4 matrix_scalar_mult(GLfloat s, mat4 m)
{
	mat4 result;
	result.x = vector_scalar_mult(s, m.x);
	result.y = vector_scalar_mult(s, m.y);
	result.z = vector_scalar_mult(s, m.z);
	result.w = vector_scalar_mult(s, m.w);
	//print_matrix(result);
	return result;
}

mat4 matrix_addition(mat4 m1, mat4 m2)
{
	mat4 result;
	result.x = vector_addition(m1.x, m2.x);
	result.y = vector_addition(m1.y, m2.y);
	result.z = vector_addition(m1.z, m2.z);
	result.w = vector_addition(m1.w, m2.w);
	return result;
}

mat4 matrix_subtraction(mat4 m1, mat4 m2)
{
	mat4 result;
	result.x = vector_subtraction(m1.x, m2.x);
	result.y = vector_subtraction(m1.y, m2.y);
	result.z = vector_subtraction(m1.z, m2.z);
	result.w = vector_subtraction(m1.w, m2.w);
	return result;
}

mat4 matrix_transpose(mat4 m)
{
	mat4 result;
	result.x = (vec4) { m.x.x, m.y.x, m.z.x, m.w.x };
	result.y = (vec4) { m.x.y, m.y.y, m.z.y, m.w.y };
	result.z = (vec4) { m.x.z, m.y.z, m.z.z, m.w.z };
	result.w = (vec4) { m.x.w, m.y.w, m.z.w, m.w.w };
	//printf("\n the original: \n");
	//print_matrix(m);
	return result;
}

mat4 matrix_matrix_mult(mat4 m1, mat4 m2)
{
	mat4 m1_t = matrix_transpose(m1);
	mat4 result = {
		(vec4) {
dot_product(m1_t.x, m2.x), dot_product(m1_t.y, m2.x), dot_product(m1_t.z, m2.x), dot_product(m1_t.w, m2.x)
},
(vec4) {
dot_product(m1_t.x, m2.y), dot_product(m1_t.y, m2.y), dot_product(m1_t.z, m2.y), dot_product(m1_t.w, m2.y)
},
(vec4) {
dot_product(m1_t.x, m2.z), dot_product(m1_t.y, m2.z), dot_product(m1_t.z, m2.z), dot_product(m1_t.w, m2.z)
},
(vec4) {
dot_product(m1_t.x, m2.w), dot_product(m1_t.y, m2.w), dot_product(m1_t.z, m2.w), dot_product(m1_t.w, m2.w)
}
	};
	//print_matrix(result);
	return result;
}

vec4 matrix_vector_mult(mat4 m, vec4 v)
{
	mat4 m_t = matrix_transpose(m);
	//print_matrix(m_t);
	vec4 result = { dot_product(m_t.x, v), dot_product(m_t.y, v), dot_product(m_t.z, v), v.w };
	//print_vector(result);
	return result;
}

GLfloat matrix3_determinant(mat3 m)
{
	return m.x.x*m.y.y*m.z.z - m.x.x*m.y.z*m.z.y - m.y.x*m.x.y*m.z.z + m.y.x*m.x.z*m.z.y + m.z.x*m.x.y*m.y.z - m.z.x*m.x.z*m.y.y;
}

GLfloat matrix4_abs_determinant(mat4 m)
{
	mat3 sub1 = { {m.y.y, m.y.z, m.y.w }, {m.z.y, m.z.z, m.z.w }, {m.w.y, m.w.z, m.w.w } };
	mat3 sub2 = { {m.x.y, m.x.z, m.x.w }, {m.z.y, m.z.z, m.z.w }, {m.w.y, m.w.z, m.w.w } };
	mat3 sub3 = { {m.x.y, m.x.z, m.x.w }, {m.y.y, m.y.z, m.y.w }, {m.w.y, m.w.z, m.w.w } };
	mat3 sub4 = { {m.x.y, m.x.z, m.x.w }, {m.y.y, m.y.z, m.y.w }, {m.z.y, m.z.z, m.z.w } };
	GLfloat determinant = m.x.x*matrix3_determinant(sub1) - m.y.x*matrix3_determinant(sub2) + m.z.x*matrix3_determinant(sub3) - m.w.x*matrix3_determinant(sub4);
	if (determinant < 0) {
		determinant = 0 - determinant;
	}
	return determinant;
}

mat4 matrix_minor(mat4 m)
{
	mat3 sub11 = { {m.y.y, m.y.z, m.y.w }, {m.z.y, m.z.z, m.z.w }, {m.w.y, m.w.z, m.w.w } };
	mat3 sub12 = { {m.x.y, m.x.z, m.x.w }, {m.z.y, m.z.z, m.z.w }, {m.w.y, m.w.z, m.w.w } };
	mat3 sub13 = { {m.x.y, m.x.z, m.x.w }, {m.y.y, m.y.z, m.y.w }, {m.w.y, m.w.z, m.w.w } };
	mat3 sub14 = { {m.x.y, m.x.z, m.x.w }, {m.y.y, m.y.z, m.y.w }, {m.z.y, m.z.z, m.z.w } };

	mat3 sub21 = { {m.y.x, m.y.z, m.y.w }, {m.z.x, m.z.z, m.z.w }, {m.w.x, m.w.z, m.w.w } };
	mat3 sub22 = { {m.x.x, m.x.z, m.x.w }, {m.z.x, m.z.z, m.z.w }, {m.w.x, m.w.z, m.w.w } };
	mat3 sub23 = { {m.x.x, m.x.z, m.x.w }, {m.y.x, m.y.z, m.y.w }, {m.w.x, m.w.z, m.w.w } };
	mat3 sub24 = { {m.x.x, m.x.z, m.x.w }, {m.y.x, m.y.z, m.y.w }, {m.z.x, m.z.z, m.z.w } };

	mat3 sub31 = { {m.y.x, m.y.y, m.y.w }, {m.z.x, m.z.y, m.z.w }, {m.w.x, m.w.y, m.w.w } };
	mat3 sub32 = { {m.x.x, m.x.y, m.x.w }, {m.z.x, m.z.y, m.z.w }, {m.w.x, m.w.y, m.w.w } };
	mat3 sub33 = { {m.x.x, m.x.y, m.x.w }, {m.y.x, m.y.y, m.y.w }, {m.w.x, m.w.y, m.w.w } };
	mat3 sub34 = { {m.x.x, m.x.y, m.x.w }, {m.y.x, m.y.y, m.y.w }, {m.z.x, m.z.y, m.z.w } };

	mat3 sub41 = { {m.y.x, m.y.y, m.y.z }, {m.z.x, m.z.y, m.z.z }, {m.w.x, m.w.y, m.w.z } };
	mat3 sub42 = { {m.x.x, m.x.y, m.x.z }, {m.z.x, m.z.y, m.z.z }, {m.w.x, m.w.y, m.w.z } };
	mat3 sub43 = { {m.x.x, m.x.y, m.x.z }, {m.y.x, m.y.y, m.y.z }, {m.w.x, m.w.y, m.w.z } };
	mat3 sub44 = { {m.x.x, m.x.y, m.x.z }, {m.y.x, m.y.y, m.y.z }, {m.z.x, m.z.y, m.z.z } };

	mat4 result = { {matrix3_determinant(sub11), -matrix3_determinant(sub21), matrix3_determinant(sub31), -matrix3_determinant(sub41)},
		{-matrix3_determinant(sub12), matrix3_determinant(sub22), -matrix3_determinant(sub32), matrix3_determinant(sub42)},
		{matrix3_determinant(sub13), -matrix3_determinant(sub23), matrix3_determinant(sub33), -matrix3_determinant(sub43)},
		{-matrix3_determinant(sub14), matrix3_determinant(sub24), -matrix3_determinant(sub34), matrix3_determinant(sub44)},
	};
	return result;
}

mat4 matrix_inverse(mat4 m)
{
	float sc = 1.0 / matrix4_abs_determinant(m);
	mat4 m_i = matrix_scalar_mult(sc, matrix_transpose(matrix_minor(m)));
	return m_i;
}

bool check_inverse_possible(mat4 m)
{
	//mat4 m_i;
	if (matrix4_abs_determinant(m) != 0.0)
	{
		return true;
	}
	else {
		printf("This matrix is not invertible.\n");
		return false;
	}
}


mat4 generate_translation_matrix(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 translation_matrix = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
	translation_matrix.w.x = x;
	translation_matrix.w.y = y;
	translation_matrix.w.z = z;
	return translation_matrix;
}

mat4 generate_scaling_matrix(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 result = { {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
	result.x.x = x;
	result.y.y = y;
	result.z.z = z;

	return result;
}

mat4 rotate_about_x(GLfloat theta) {
	mat4 result = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
	result.y.y = cosf(theta);
	result.y.z = sinf(theta);
	result.z.y = -sinf(theta);
	result.z.z = cosf(theta);
	return result;
}

mat4 rotate_about_y(GLfloat theta) {
	mat4 result = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
	result.x.x = cosf(theta);
	result.x.z = -sinf(theta);
	result.z.x = sinf(theta);
	result.z.z = cosf(theta);
	return result;
}

mat4 rotate_about_z(GLfloat theta) {
	mat4 result = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
	result.x.x = cosf(theta);
	result.x.y = sinf(theta);
	result.y.x = -sinf(theta);
	result.y.y = cosf(theta);
	return result;
}


mat4 look_at(GLfloat eyex, GLfloat eyey, GLfloat eyez,
	GLfloat atx, GLfloat aty, GLfloat atz,
	GLfloat upx, GLfloat upy, GLfloat upz){	vec4 atPoint = {atx, aty, atz, 0.0 };	vec4 eyeVector = {(eyex-atx), (eyey-aty), (eyez-atz), 0};	vec4 upVector = {upx, upy, upz, 0.0};		vec4 n_f = vector_scalar_mult(1.0 / sqrt((double)(eyeVector.x*eyeVector.x + eyeVector.y*eyeVector.y + eyeVector.z*eyeVector.z)), eyeVector);	vec4 u = cross_product(upVector, n_f);	vec4 u_f = vector_scalar_mult(1.0 / sqrt((double)(u.x*u.x + u.y*u.y + u.z*u.z)), u);	GLfloat d = sqrt((double)(eyex*eyex + eyey*eyey + eyez*eyez));	vec4 t1 = cross_product(n_f, u_f);	vec4 v_f = vector_scalar_mult(1.0 / sqrt((double)(t1.x*t1.x + t1.y*t1.y + t1.z*t1.z)), t1);	mat4 m = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };	m.x.x = u_f.x;	m.x.y = v_f.x;	m.x.z = n_f.x;	m.x.w = eyex;	m.y.x = u_f.y;	m.y.y = v_f.y;	m.y.z = n_f.y;	m.y.w = eyey;	m.z.x = u_f.z;	m.z.y = v_f.z;	m.z.z = n_f.z;	m.z.w = eyez;	mat4 m_t = matrix_transpose(m);	mat4 result = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };	mat4 m_t_inverse = matrix_inverse(m_t);	return m_t_inverse;}mat4 frustum(GLfloat left, GLfloat right,
	GLfloat bottom, GLfloat top,
	GLfloat near_value, GLfloat far_value) {	mat4 result = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 0.0} };
	result.x.x = (-2 * near_value) / (right - left);
	result.y.y = (-2 * near_value) / (top - bottom);
	result.z.x = (left + right) / (right - left);
	result.z.y = (bottom + top) / (top - bottom);
	result.z.z = (far_value + near_value) / (far_value - near_value);
	result.z.w = -1;
	result.w.z = -(2 * near_value * far_value) / (far_value - near_value);
	return result;}