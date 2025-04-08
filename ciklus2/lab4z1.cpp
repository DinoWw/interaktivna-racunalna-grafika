//*********************************************************************************
//	Define convex polygon with clockwise vertex order and fill it in
//	Right click to check if point is within polygon
//*********************************************************************************


#include <stdio.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <math.h>

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
// void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void myLine(GLint xa, GLint ya, GLint xb, GLint yb);

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
	FILE *file = fopen("objects/teddy.obj", "r");
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




	// CALCULATE A, B, C and D

	A = (GLdouble*) malloc(faceCount * sizeof(GLdouble));
	B = (GLdouble*) malloc(faceCount * sizeof(GLdouble));
	C = (GLdouble*) malloc(faceCount * sizeof(GLdouble));
	D = (GLdouble*) malloc(faceCount * sizeof(GLdouble));

	for(int i = 0; i < faceCount; i++) {
		GLdouble *p1 = faces[3 * i + 0];
		GLdouble *p2 = faces[3 * i + 1];
		GLdouble *p3 = faces[3 * i + 2];

		A[i] =   (p2[1]-p1[1]) * (p3[2]-p1[2]) - (p2[2]-p1[2]) * (p3[1]-p1[1]);
		B[i] = - (p2[0]-p1[0]) * (p3[2]-p1[2]) + (p2[2]-p1[2]) * (p3[0]-p1[0]);
		C[i] =   (p2[0]-p1[0]) * (p3[1]-p1[1]) - (p2[1]-p1[1]) * (p3[0]-p1[0]);
		D[i] = - p1[0] * A[i] - p1[1] * B[i] - p1[2] * C[i];

	}

	// Ucitaj koordinate ispitne tocke V
	GLdouble p[3];
	printf("Unesi koordinate ispitne tocke: \n");
	scanf("%lf %lf %lf", &p[0], &p[1], &p[2]);
	int inside = 1;
	for(int i = 0; i < faceCount; i++){
		glm::vec4 R(A[i], B[i], C[i], D[i]);
		glm::vec4 V(p[0], p[1], p[2], 0);
		if(glm::dot(V, R) > 0){
			// printf("V %f %f %f 0\n", V[0], V[1], V[2]);
			// printf("R %f %f %f %f\n", A[i], B[i], C[i], D[i]);
			// printf("%f\n", glm::dot(V, R));
			inside = 0;
			break;
		}
	}
	printf("The point is %s the body.\n", inside ? "inside":"outside");
	
	// INITIALIZE GLUT ENVIRONMENT
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Glut OpenGL Konveksno Tijelo");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	// glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);


	// printf("Tipke r, g, b, k mijenjaju boju.\n");
		

	glutMainLoop();
	return 0;
}


void drawBody() {
	
	// glBegin(GL_TRIANGLES);
	// for(int f = 1; f < 3 * faceCount; f+=1) {
	// 	// printf("Vertex %d: %f %f %f\n", f, faces[f][0], faces[f][1], faces[f][2]);
	// 	glVertex3d(faces[f][0], faces[f][1], faces[f][2]);
	// 	glVertex3d(faces[f-1][0], faces[f-1][1], faces[f-1][2]);
	// }
	// glEnd();

	glBegin(GL_LINES);
	for(int f = 0; f < 3 * faceCount; f+=3) {
		// printf("Vertex %d: %f %f %f\n", f, faces[f][0], faces[f][1], faces[f][2]);
		glVertex3d(faces[f][0], faces[f][1], faces[f][2]);
		glVertex3d(faces[f+1][0], faces[f+1][1], faces[f+1][2]);
		glVertex3d(faces[f+1][0], faces[f+1][1], faces[f+1][2]);
		glVertex3d(faces[f+2][0], faces[f+2][1], faces[f+2][2]);
		glVertex3d(faces[f+2][0], faces[f+2][1], faces[f+2][2]);
		glVertex3d(faces[f][0], faces[f][1], faces[f][2]);
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
	/* 
	glPushMatrix();
	gluLookAt((GLdouble)0.f, (GLdouble)0.f, (GLdouble)10.f, (GLdouble)0.f, (GLdouble)0.f, (GLdouble)0.f, (GLdouble)0.f, (GLdouble)1.f, (GLdouble)0.f);

	// glTranslated(0, 0, -10);

	glColor3d(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex3d(0, 0, 0);
	glVertex3d(100, 100, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(100, 100, 0);
	glEnd();


	glPopMatrix();
 */

	glPushMatrix();

	// glRotatef(30.0f, 1.0f, 1.0f, 1.0f);
	// printf("%f %f %f\n", vmax[0]-vmin[0], vmax[1]-vmin[1], vmax[2]-vmin[2]);

	// glTranslatef(0.f, 0.f, -1.f);

	
	// Draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, -20.0f, -100.0f);
		glVertex3f(-100.0f, -20.0f,  100.0f);
		glVertex3f( 100.0f, -20.0f,  100.0f);
		glVertex3f( 100.0f, -20.0f, -100.0f);
	glEnd();

	// glTranslated(2,2, 0);

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
	width = w; height = h;               //promjena sirine i visine prozora
	glViewport(0, 0, width, height);	//  otvor u prozoru
	
	glMatrixMode(GL_PROJECTION);		//	matrica projekcije
	glLoadIdentity();					//	jedinicna matrica
	glFrustum(-1, 1, -1, 1, 8, 1000);
	// gluPerspective(120.0f, ((GLdouble) w)/h, 1.f, 1000.f);
	// glOrtho(0, width, 0, height, -1, 1); 	//	okomita projekcija
	glMatrixMode(GL_MODELVIEW);			//	matrica pogleda
	glLoadIdentity();					//	jedinicna matrica
	gluLookAt(
		(GLdouble)0.f, (GLdouble)0.f, (GLdouble)10.f, 
		(GLdouble)0.f, (GLdouble)0.f, (GLdouble)0.f, 
		(GLdouble)0.f, (GLdouble)1.f, (GLdouble)0.f);
	// glTranslatef(-( vmax[0]-vmin[0] ) / 2, -( vmax[1]-vmin[1] ) / 2, -( vmax[2]-vmin[2] ) / 2);
	// glScalef( 1 / ( vmax[0]-vmin[0] ), 1 / ( vmax[1]-vmin[1] ), 1 / ( vmax[2]-vmin[2] ) );

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
	glPointSize(10.0);					//	postavi velicinu tocke za liniju
	glLineWidth(1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);		//	postavi boju linije
}





//*********************************************************************************
//	Tastatura tipke - r, g, b, k - mijenjaju boju.
//*********************************************************************************

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	switch (theKey)
	{
    case 'c':
        myReshape(width, height);
        break;

    case 'r':
        glColor3f(1, 0, 0);
        break;
        
	case 'g':
		glColor3f(0, 1, 0);
		break;

	case 'b':
		glColor3f(0, 0, 1);
		break;

	case 'k':
		glColor3f(0, 0, 0);

	}
	glRecti(width - 15, height - 15, width, height); // crta mali kvadrat u boji
	glFlush();
}

