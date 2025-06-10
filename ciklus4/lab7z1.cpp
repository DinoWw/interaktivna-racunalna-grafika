//*************************************************************************************************************
//	Crtanje tijela
//	Tijelo se crta u funkciji myObject
//	
//	Zadatak: Treba ucitati tijelo zapisano u *.obj, sjencati i ukloniti staznje poligone
//	S tastature l - pomicanje ocista po x osi +
//		k - pomicanje ocista po x osi +
//              r - pocetni polozaj
//              esc izlaz iz programa
//*************************************************************************************************************

#include <stdio.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <math.h>

//*********************************************************************************
//	Pokazivac na glavni prozor i pocetna velicina.
//*********************************************************************************

GLuint window; 
GLuint width = 300, height = 300;

typedef struct _Ociste {
	GLdouble	x;
	GLdouble	y;
	GLdouble	z;
} Ociste;

typedef struct _Vec {
	GLdouble	x;
	GLdouble	y;
	GLdouble	z;
} Vec;


// Globalne varijable

Ociste ociste = {0, 0, 5.0};

GLdouble vmax[3] = {0, 0, 0};
GLdouble vmin[3] = {99999999, 99999999, 99999999};

int vertCount = 0, faceCount = 0;

GLdouble *verts;
GLdouble **faces;
Vec *fNormals;
Vec *vNormals;


//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay		();
void myReshape		(int width, int height);
void myMouse		(int button, int state, int x, int y);
void myKeyboard		(unsigned char theKey, int mouseX, int mouseY);
void myObject		();
void redisplay_all	(void);


//*********************************************************************************
//	Glavni program.
//*********************************************************************************

int main(int argc, char** argv)
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
	fNormals = (Vec*) malloc(3 * faceCount * sizeof(Vec));
	vNormals = (Vec*) malloc(3 * vertCount * sizeof(Vec));
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

	// calculate face normals

	for(int f = 0; f < faceCount; f+=1) {

		GLdouble *pc = faces[3 * f + 0];
		GLdouble *pb = faces[3 * f + 1];
		GLdouble *pa = faces[3 * f + 2];

		Vec v1 = {
			pa[0] - pb[0],
			pa[1] - pb[1],
			pa[2] - pb[2]
		};
		Vec v2 = {
			pc[0] - pb[0],
			pc[1] - pb[1],
			pc[2] - pb[2]
		};

		fNormals[f] = {
				v1.y*v2.z - v1.z*v2.y,
			- (	v1.x*v2.z - v1.z*v2.x ),
				v1.x*v2.y -	v1.y*v2.x
		};
	}


	
	// postavljanje dvostrukog spremnika za prikaz (zbog titranja)
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize (width, height); 
	glutInitWindowPosition (100, 100);
	glutInit(&argc, argv);
   
	window = glutCreateWindow ("Tijelo");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	printf("Tipka: l - pomicanje ocista po x os +\n");
	printf("Tipka: k - pomicanje ocista po x os -\n");
	printf("Tipka: r - pocetno stanje\n");
	printf("esc: izlaz iz programa\n");

	glutMainLoop();
	return 0;
}

//*********************************************************************************
//	Osvjezavanje prikaza. 
//*********************************************************************************

void myDisplay(void)
{
	// printf("Pozvan myDisplay()\n");
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f);		         // boja pozadine - bijela
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	myObject ();
	glutSwapBuffers();      // iscrtavanje iz dvostrukog spemnika (umjesto glFlush)
}

//*********************************************************************************
//	Promjena velicine prozora.
//	Funkcija gluPerspective i gluLookAt se obavlja 
//		transformacija pogleda i projekcija
//*********************************************************************************

void myReshape (int w, int h)
{
	// printf("MR: width=%d, height=%d\n",w,h);
	width=w; height=h;
	glViewport (0, 0, width, height); 
	glMatrixMode (GL_PROJECTION);        // aktivirana matrica projekcije
	glLoadIdentity ();
	gluPerspective(45.0, (float)width/height, 0.5, 8.0); // kut pogleda, x/y, prednja i straznja ravnina odsjecanja
	gluLookAt (ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	// ociste x,y,z; glediste x,y,z; up vektor x,y,z
	glMatrixMode (GL_MODELVIEW);         // aktivirana matrica modela
}

void updatePerspective()
{
	glMatrixMode (GL_PROJECTION);        // aktivirana matrica projekcije
	glLoadIdentity ();
	gluPerspective(45.0, (float)width/height, 0.5, 8.0); // kut pogleda, x/y, prednja i straznja ravnina odsjecanja
	gluLookAt (ociste.x, ociste.y, ociste.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	// ociste x,y,z; glediste x,y,z; up vektor x,y,z
	glMatrixMode (GL_MODELVIEW);         // aktivirana matrica modela
}

//*********************************************************************************
//	Crta moj objekt. Ovdje treba naciniti prikaz ucitanog objekta.
//*********************************************************************************

void myObject ()
{
//	glutWireCube (1.0);
//	glutSolidCube (1.0);
//	glutWireTeapot (1.0);
//	glutSolidTeapot (1.0);
	
	// glBegin (GL_TRIANGLES); // ili glBegin (GL_LINE_LOOP); za zicnu formu
	// 	glColor3ub(255, 0, 0);	glVertex3f(-1.0, 0.0, 0.0);
	// 	glColor3ub(0, 0, 0);	glVertex3f(0.0, 1.0, 0.0);
	// 	glColor3ub(100, 0, 0);	glVertex3f(0.0, 0.0, 1.0);
	// glEnd();
	
	glColor3ub(0, 0, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for(int f = 0; f < faceCount; f+=1) {
		// printf("Vertex %d: %f %f %f\n", f, faces[3*f][0], faces[3*f][1], faces[3*f][2]);

		// backface culling
		Vec normal = fNormals[f];

		GLdouble cross = ociste.x*normal.x + ociste.y*normal.y + ociste.z*normal.z;

		if(cross < 0) {
			continue;
		}

		glVertex3d(faces[3*f][0], faces[3*f][1], faces[3*f][2]);
		glVertex3d(faces[3*f+1][0], faces[3*f+1][1], faces[3*f+1][2]);
		glVertex3d(faces[3*f+2][0], faces[3*f+2][1], faces[3*f+2][2]);
	}
	glEnd();


}

//*********************************************************************************
//	Mis.
//*********************************************************************************

void myMouse(int button, int state, int x, int y)
{
	//	Desna tipka - brise canvas. 
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		ociste.x=0;
		updatePerspective();
		glutPostRedisplay();
	}
}

//*********************************************************************************
//	Tastatura tipke - esc - izlazi iz programa.
//*********************************************************************************

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	switch (theKey) 
	{
		case 'l': ociste.x = ociste.x+0.1;
        break;

		case 'k': ociste.x =ociste.x-0.1;
        break;

		case 'r': ociste.x=0.0;
        break;

		case 27:  exit(0);
		break;
	}
	updatePerspective();
	glutPostRedisplay();
}

