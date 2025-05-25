
#include <stdio.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <math.h>

void coutVec4(const char* name, glm::vec4 vec) {
	printf("%10s [ %14.4f, %14.4f, %14.4f, %14.4f ]\n", name, vec.x, vec.y, vec.z, vec[3]);
}

//*********************************************************************************
//	Pokazivac na glavni prozor i pocetna velicina.
//*********************************************************************************

GLuint window;
GLuint width = 300, height = 300;

//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myReshape(int width, int height);

void controlFunction(unsigned char theKey, int mouseX, int mouseY);


void drawBody();

// Globalne varijable

GLdouble vmax[3] = {0, 0, 0};
GLdouble vmin[3] = {99999999, 99999999, 99999999};

int vertCount = 0, faceCount = 0;

GLdouble *verts;
GLdouble **faces;
GLdouble *A, *B, *C, *D;

//*********************************************************************************
//	Glavni program.
//*********************************************************************************

int main(int argc, char ** argv)
{
	// count vertices and faces

	// open obj file
	FILE *file = fopen("../objects/teddy.obj", "r");
	if(file == NULL) { 
		printf("Failed to open file.\n");
		exit(1);
	}
	// read lines
	char line[300];
	while( fgets(line, sizeof(line), file) != NULL ) {
		switch(line[0]){
		case('v'):
			vertCount++;
			break;
		case('f'):
			faceCount++;
			break;
		case('#'):
		default:
			break;
		}
	}


	// Load vertices and faces
	// open obj file
	rewind(file);

	verts = (GLdouble*) malloc(3 * vertCount * sizeof(GLdouble));
	faces = (GLdouble**) malloc(3 * faceCount * sizeof(GLdouble*));
	vertCount = 0;
	faceCount = 0;

	// read lines
	GLdouble *start;
	while( fgets(line, sizeof(line), file) != NULL ) {
		//printf("Line: %s", line);
		switch(line[0]){
		case('#'):
			break;
		case('v'):
			// process vertex
			start = &verts[vertCount * 3];
			sscanf(line, "v %lf %lf %lf", start+0, start+1, start+2 );
			vertCount++;
			for(int i = 0; i < 3; i++) {
				if(vmax[i] < *(start+i)) vmax[i] = *(start+i);
				if(vmin[i] > *(start+i)) vmin[i] = *(start+i);
			}
			break;
		case('f'):
			// process face
			int vertIds[3];
			sscanf(line, "f %d %d %d", &vertIds[0], &vertIds[1], &vertIds[2]);
			// printf("Face vertices: %d %d %d\n", vertIds[0], vertIds[1], vertIds[2]);
			faces[3 * faceCount + 0] = &verts[3 * ( vertIds[0]-1 )];
			faces[3 * faceCount + 1] = &verts[3 * ( vertIds[1]-1 )];
			faces[3 * faceCount + 2] = &verts[3 * ( vertIds[2]-1 )];
			faceCount ++;
			break;

		}
	}
	
	//printf("Max xyz %f, %f, %f\n", vmax[0], vmax[1], vmax[2]);
	//printf("Min xyz %f, %f, %f\n", vmin[0], vmin[1], vmin[2]);

	// close obj file
	fclose(file);

	// testing
	//printf("Vertices %f %f %f\n", faces[0][0], faces[0][1], faces[0][2]);
	//printf("Vertices %f %f %f\n", faces[1][0], faces[1][1], faces[1][2]);
	//printf("Vertices %f %f %f\n", faces[2][0], faces[2][1], faces[2][2]);

	// CENTER AND SCALE BODY

	GLdouble movex = (vmax[0] - vmin[0]) /2.f;
	GLdouble movey = (vmax[1] - vmin[1]) /2.f;
	GLdouble movez = (vmax[2] - vmin[2]) /2.f;
	GLdouble scalex = 2.f/ (vmax[0] - vmin[0]);
	GLdouble scaley = 2.f/ (vmax[1] - vmin[1]);
	GLdouble scalez = 2.f/ (vmax[2] - vmin[2]);

	// printf("%f %f %f %f %f %f\n", movex, movey, movez, scalex, scaley, scalez);
	for(int v = 0; v < 3 * vertCount; v+=3) {
		verts[v+0] = (verts[v+0] - movex - vmin[0] ) * scalex;
		verts[v+1] = (verts[v+1] - movey - vmin[1] ) * scaley;
		verts[v+2] = (verts[v+2] - movez - vmin[2] ) * scalez;
	}

	// INITIALIZE GLUT ENVIRONMENT
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Glut OpenGL Perspektivna Projekcija");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(controlFunction);


	// printf("Tipke r, g, b, k mijenjaju boju.\n");
		

	glutMainLoop();
	return 0;
}



glm::vec3 O = {100, 0, 100};
glm::vec3 G = {0, 0, 0};
glm::vec3 Vup = {0, 1, 0};
void drawBody() {
	
	using glm::vec4;
	using glm::vec3;
	using glm::vec2;
	using glm::mat4x4;
	Vup = glm::normalize(Vup);

	mat4x4 T1 = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-O[0], -O[1], -O[2], 1,
	};
	T1 = glm::transpose(T1);

	vec3 z = glm::normalize(G-O);
	vec3 x = glm::cross(z, Vup);
	vec3 y = glm::cross(x, z);
    
	mat4x4 Ruk = {
		x[0], y[0], z[0], 0,
		x[1], y[1], z[1], 0,
		x[2], y[2], z[2], 0,
		0, 0, 0, 1,
	};
	Ruk = glm::transpose(Ruk);

	mat4x4 Tz = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	Tz = glm::transpose(Tz);

	mat4x4 T = T1 * Ruk * Tz;
	// mat4x4 T = Tz * Ruk * T1;


	GLdouble H = glm::distance(O, G);

	mat4x4 P = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 1/H,
		0, 0, 0, 0
	};
	P = glm::transpose(P);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for(int f = 0; f < 3 * faceCount; f+=3) {
		// printf("Vertex %d: %f %f %f\n", f, faces[f][0], faces[f][1], faces[f][2]);
		for(int i = 0; i < 3; i ++) {
			vec4 p = {faces[f + i][0], faces[f + i][1], faces[f + i][2], 1};
			vec4 pT = p*T* P;
			// coutVec4("p", p);
			// coutVec4("pT", pT);

			// glVertex3d(pT[0] / pT[3] * 100, pT[1] / pT[3] * 100, pT[2] / pT[3] * 100);
			// glVertex3d(pT[0] / pT[3], pT[1] / pT[3], pT[2] / pT[3]);
			// glVertex3d(faces[f + i][0] * 100, faces[f + i][1] * 100, faces[f + i][2] * 100);
			glVertex2d(pT.x / pT.w, pT.y / pT.w);
		}
	}
	glEnd();

}


//*********************************************************************************
//	Osvjezavanje prikaza. (nakon preklapanja prozora) 
//*********************************************************************************

void myDisplay()
{
	// printf("Pozvan myDisplay()\n");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //  boja pozadine
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, -20.0f, -100.0f);
		glVertex3f(-100.0f, -20.0f,  100.0f);
		glVertex3f( 100.0f, -20.0f,  100.0f);
		glVertex3f( 100.0f, -20.0f, -100.0f);
	glEnd();

	glColor4f(0.f, 0.5f, 0.f, 0.1f);
	drawBody();
	
	glPopMatrix();

	glFlush(); 
}

//*********************************************************************************
//	Promjena velicine prozora.
//*********************************************************************************

void myReshape(int w, int h)
{
	// printf("Pozvan myReshape()\n");
	width = w; height = h;              // promjena sirine i visine prozora
	glViewport(0, 0, width, height);	//  otvor u prozoru
	
	glMatrixMode(GL_PROJECTION);		//	matrica projekcije
	glLoadIdentity();					//	jedinicna matrica
	// glOrtho(0, width, 0, height, -100, 100); 	//	okomita projekcija

	glMatrixMode(GL_MODELVIEW);			//	matrica pogleda
	glLoadIdentity();					//	jedinicna matrica

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
	glPointSize(10.0);					//	postavi velicinu tocke za liniju
	glLineWidth(1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);		//	postavi boju linije
}



void controlFunction(unsigned char theKey, int mouseX, int mouseY)
{
	static glm::vec3* point = &O;
	static double speed = 1; 

	switch (theKey)
	{
	case 'o':
		point = &O;
		speed = 1;
		break;

	case 'g':
		point = &G;
		speed = 0.1;
		break;

	case 'h':
		point->x += speed;
		break;

	case 'j':
		point->y -= speed;
		break;

	case 'k':
		point->y += speed;
		break;

	case 'l':
		point->x -= speed;
		break;

	case 'u':
		point->z += speed;
		break;

	case 'i':
		point->z -= speed;
		break;

	}
	
	glutPostRedisplay();
}
