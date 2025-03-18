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

	glm::vec3 v1 = glm::vec3(2, 3, -4) + glm::vec3(-1, 4, -1);
	float s = glm::dot( v1 , glm::vec3(-1, 4, -1) );
	glm::vec3 v2 = glm::cross( v1, glm::vec3(2, 2, 4));
	glm::vec3 v3 = glm::normalize( v2 );
	glm::vec3 v4 = - v2;

	coutVec3("v1", v1);
	printf("%10s %14.4f\n", "s", s);
	coutVec3("v2",v2);
	coutVec3("v3", v3);
	coutVec3("v4", v4);

	glm::mat3 MA = glm::mat3(
		1,	2,	3,
		2,	1,	3,
		4,	5,	1
	);
	glm::mat3 MB = glm::mat3(
		-1,	2,	-3,
		5,	-2,	7,
		-4,	-1,	3
	);

	glm::mat3 M1 = MA + MB;
	glm::mat3 M2 = MA + glm::transpose(MB);
	glm::mat3 M3 = MA + glm::inverse(MB);

	printf("\n");
	coutMat3("MA", MA);
	coutMat3("MB", MB);

	printf("\n");
	coutMat3("M1", M1);
	coutMat3("M2", M2);
	coutMat3("M3", M3);


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


