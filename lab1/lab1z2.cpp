#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <GL/freeglut.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale


void coutVec3(const char* name, glm::vec3 vec);
void coutMat3(const char* name, glm::mat3 mat);

int main(int argc, char** argv) {

	// glm::mat3 A = glm::mat3(
	// 	1,	1,	1,
	// 	-1,	-2,	1,
	// 	2,	1,	3
	// );
	// glm::vec3 b = glm::vec3(6, -2, 13);


	glm::vec3 b = glm::vec3();
	glm::mat3 A = glm::mat3();
	for( int i = 0; i < 3; i++) {
		printf("_x + _y + _z = _ :  ");
		scanf("%f %f %f %f", &A[i][0], &A[i][1], &A[i][2], &b[i]);
		printf("%.1fx + %.1fy + %.1f1z = %.1f\n", A[i][0], A[i][1], A[i][2], b[i]);
	}
	coutMat3("A", A);
	coutVec3("b", b);

	A = glm::transpose(A);

	glm::vec3 xyz = glm::inverse(A) * b;

	coutVec3("xyz", xyz);


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



