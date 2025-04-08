//*********************************************************************************
//	Define convex polygon with clockwise vertex order and fill it in
//	Right click to check if point is within polygon
//*********************************************************************************

#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

//*********************************************************************************
//	Tocke poligona
//*********************************************************************************

GLdouble Px[100], Py[100];
int pointCount = 0;

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
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void myLine(GLint xa, GLint ya, GLint xb, GLint yb);

void myPolygon(GLdouble px[], GLdouble py[], int pointcount);
void checkPoint(int x, int y);


//*********************************************************************************
//	Glavni program.
//*********************************************************************************

int main(int argc, char ** argv)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Glut OpenGL Konveksni Poligon");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	printf("Lijevom tipkom misa zadaj tocke - konveskni poligon\n");
	printf("Desnom tipkom misa testiraj tocku\n");
	printf("Tipke r, g, b, k mijenjaju boju.\n");
	printf("Tipke r, g, b, k mijenjaju boju.\n");
		
	glutMainLoop();
	return 0;
}


//*********************************************************************************
//	Osvjezavanje prikaza. (nakon preklapanja prozora) 
//*********************************************************************************

void myDisplay()
{
	//printf("Pozvan myDisplay()\n");
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //  boja pozadine
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //brisanje nakon svake iscrtane linije
	glFlush();
}

//*********************************************************************************
//	Promjena velicine prozora.
//*********************************************************************************

void myReshape(int w, int h)
{
	//printf("Pozvan myReshape()\n");
	width = w; height = h;               //promjena sirine i visine prozora
	pointCount = 0;								//	indeks tocke 0-prva 1-druga tocka
	glViewport(0, 0, width, height);	//  otvor u prozoru
	
	glMatrixMode(GL_PROJECTION);		//	matrica projekcije
	glLoadIdentity();					//	jedinicna matrica
	glOrtho(0, width, 0, height, -1, 1); 	//	okomita projekcija
	glMatrixMode(GL_MODELVIEW);			//	matrica pogleda
	glLoadIdentity();					//	jedinicna matrica

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
	glPointSize(1.0);					//	postavi velicinu tocke za liniju
	glColor3f(0.0f, 0.0f, 0.0f);		//	postavi boju linije
}

void abc(GLdouble px[], GLdouble py[], int pointcount, GLdouble a[], GLdouble b[], GLdouble c[]) {
	for(int i = 0; i < pointcount; i ++) {
		a[i] = py[i] - py[i+1];
		b[i] = -px[i] + px[i+1];
		c[i] = px[i] * py[i+1] - px[i+1] * py[i];
	}
}

void checkPoint(int x, int y) {

	GLdouble a[pointCount], b[pointCount], c[pointCount];
	abc(Px, Py, pointCount, a, b, c);
	
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	int valid = 1;
	for(int i = 0; i < pointCount; i ++) {
		if(x * a[i] + y * b[i] + c[i] > 0) {
			valid = 0;
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
	}

	printf("Point (%d, %d) is %s the polygon.\n", x, y, valid ? "inside" : "outside");
	
	myPolygon(Px, Py, pointCount);
	glutPostRedisplay();				//  obnovi crtanje scene

}

//*********************************************************************************
//	Crtaj moju liniju.
//*********************************************************************************

void myPolygon(GLdouble px[], GLdouble py[], int pointcount)
{
	if(pointcount < 3)
		return;
    glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINE_LOOP);
	GLdouble maxX, maxY = 0;
	GLdouble minX, minY = 999999999;	// vece od bilo koje relolucije ekrana
	// first point is duplicated to the last one so no edge cases
	for(int i = 0; i < pointcount; i ++) {
		glVertex2d(px[i], py[i]);			//	crtanje poligona
		if(px[i] > maxX) maxX = px[i];
		if(py[i] > maxY) maxY = py[i];
		if(px[i] < minX) minX = px[i];
		if(py[i] < minY) minY = py[i];
	}
	glEnd();

	GLdouble a[pointcount], b[pointcount], c[pointcount];
	abc(px, py, pointcount, a, b, c);

	// 	bojanje
	glBegin(GL_LINES);
	for(int y0 = minY; y0 < maxY; y0++) {
		int L = minX;
		int D = maxX;

		for(int i = 0; i < pointcount; i ++) {
			if(a[i] == 0) continue;
			int x1 = ( -b[i] * y0 - c[i] ) / a[i];
			// lijevi brid
			if(py[i] < py[i+1]){
				if(x1 > L)	L = x1;
			} 
			// desni brid
			else {
				if(x1 < D)	D = x1;
			}
		}
		if(L < D) {
			glVertex2i(L, y0);
			glVertex2i(D, y0);
		}
	}
	glEnd();
}

//*********************************************************************************
//	Mis.
//*********************************************************************************

void myMouse(int button, int state, int x, int y)
{
	//	Lijeva tipka - crta pocetnu tocku ili liniju.
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)

	{
		//	Pamti tocke poligona.
		Px[pointCount] = x;							//	upisi tocku
		Py[pointCount] = height - y;
		pointCount ++;								//	iduca tocka
		Px[pointCount] = Px[0];							//	kopiraj prvu tocku na jednu nakon zadnje
		Py[pointCount] = Py[0];
		
    	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		myPolygon(Px, Py, pointCount);
		glutPostRedisplay();				//  obnovi crtanje scene
		printf("Koordinate tocke %d: %d %d \n", x, y);

		// glFlush();
	}

	//	Desna tipka - provjerava tocku. 
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		checkPoint(x, height-y);
	}
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

