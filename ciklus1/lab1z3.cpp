#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <GL/freeglut.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale

#define EPSILON std::numeric_limits<float>::epsilon()

bool areSame(double a, double b);

void coutVec3(const char* name, glm::vec3 vec);
void coutMat3(const char* name, glm::mat3 mat);



int main(int argc, char** argv) {

	const char vecNames[4][2] = {"A", "B", "C", "T"};
	glm::vec3 vecs[4];

	for( int i = 0; i < 4; i++) {
		printf("%sx %sy %sz :  ", vecNames[i], vecNames[i], vecNames[i]);
		scanf("%f %f %f", &vecs[i][0], &vecs[i][1], &vecs[i][2]);
		// printf("%fx + %fy + %fz = %f :  ", A[i][0], A[i][1], A[i][2], b[i]);
	}
	glm::mat3 A = glm::mat3(vecs[0], vecs[1], vecs[2]);
	glm::vec3 T = vecs[3];
	coutMat3("A", A);
	coutVec3("T", T);

	// A = glm::transpose(A);

	glm::vec3 t = glm::inverse(A) * T;

	if(areSame(t.x + t.y + t.z, 1.0f))
		coutVec3("t1 t2 t3", t);
	else
		printf("T ne lezi na trokutu\n");


}

void coutVec3(const char* name, glm::vec3 vec) {
	printf("%10s [ %14.4f, %14.4f, %14.4f ]\n", name, vec.x, vec.y, vec.z);
}
void coutMat3(const char* name, glm::mat3 mat) {
	printf(   "%10s ┌ %14.4f, %14.4f, %14.4f ┐\n"
		"           │ %14.4f, %14.4f, %14.4f │\n"
		"           └ %14.4f, %14.4f, %14.4f ┘\n", 
		name, mat[0].x, mat[0].y, mat[0].z, mat[1].x, mat[1].y, mat[1].z, mat[2].x, mat[2].y, mat[2].z);
}


bool areSame(double a, double b)
{
    return fabs(a - b) < EPSILON;
}