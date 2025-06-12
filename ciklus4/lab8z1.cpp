
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#define FRAC_MANDELBROT 0
#define FRAC_JULIA 1
#define FRAC_N 2

typedef struct _complex {
    double re;
    double im;
} complex;  


GLuint window;
GLuint width = 600, height = 300;


double eps;
double m;

int frac = FRAC_MANDELBROT;
complex default_complex = {0, 0};

complex sq(complex c) {
    return (complex) {
        .re = c.re*c.re - c.im*c.im,
        .im = 2*c.re*c.im
    };
}

complex add(complex a, complex b) {
    return {a.re+b.re, a.im+b.im};
}

int divergence_test_mandelbrot(complex c) {
    int k = -1;
    complex z = {0, 0};
    double r = 0;
    while(r < eps && k < m) {
        k++;
        z = add(sq(z), c);
        r = sqrt(z.re*z.re + z.im*z.im);
    } 

    return k;
}
int divergence_test_julia(complex c) {
    int k = -1;
    complex z = default_complex;
    double r = 0;
    while(r < eps && k < m) {
        k++;
        z = add(sq(z), c);
        r = sqrt(z.re*z.re + z.im*z.im);
    } 

    return k;
}

void renderScene( ) {
    double xmin = 0 ;
    double xmax = width-1 ;
    double ymin = 0 ;
    double ymax = height-1 ;
    double umin = -1.5;
    double umax = 0.5 ;
    double vmin = -1;
    double vmax = 1 ;
    // glColor3f( 1.0, 0, 0);//(double)n/16.f, 0, 0 ) ;

    // glBegin(GL_TRIANGLE_STRIP);
    // glVertex2i(-10, 10);
    // glVertex2i(-10, -10);
    // glVertex2i(10, -10);
    // glVertex2i(10, 10);
    // glVertex2i(30, 30);

    // glEnd();
    
    if(FRAC_MANDELBROT == frac) {
        glPointSize(1);
        glBegin(GL_POINTS ) ;
        for( int y = ymin ; y <= ymax ; y++) {
            for( int x = xmin ; x <= xmax ; x++) {
                complex c ;
                c.re = ( x-xmin ) / ( double ) ( xmax-xmin ) * ( umax-umin)+umin ;
                c.im = ( y-ymin ) / ( double ) ( ymax-ymin ) * ( vmax-vmin)+vmin ;
                int n = divergence_test_mandelbrot( c ) ;
                glColor3f((double)n/m, (double)n/m*2, 0 ) ;
                // glColor3f(100.f, 100.f, 100.f);
                glVertex2i( x , y ) ;
            }
        }
        glEnd();

    }
    else if(FRAC_JULIA == frac) { 
        glPointSize(1);
        glBegin(GL_POINTS ) ;
        for( int y = ymin ; y <= ymax ; y++) {
            for( int x = xmin ; x <= xmax ; x++) {
                complex c ;
                c.re = ( x-xmin ) / ( double ) ( xmax-xmin ) * ( umax-umin)+umin ;
                c.im = ( y-ymin ) / ( double ) ( ymax-ymin ) * ( vmax-vmin)+vmin ;
                int n = divergence_test_julia( c ) ;
                glColor3f((double)n/m, (double)n/m*2, 0 ) ;
                // glColor3f(100.f, 100.f, 100.f);
                glVertex2i( x , y ) ;
            }
        }
        glEnd();
    }

    glFlush();

}

void myReshape (int w, int h)
{
	//printf("Pozvan myReshape()\n");
	width = w; height = h;               //promjena sirine i visine prozora
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


void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    frac = (frac + 1) % FRAC_N;
    glutPostRedisplay();
}


int main(int argc, char ** argv)
{
	printf("Unesi eps i m:\n");
	scanf("%lf %lf", &eps, &m);
	printf("Unesi c.re i c.im (za Julijev skup):\n");
	scanf("%lf %lf", &default_complex.re, &default_complex.im);


	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);

	window = glutCreateWindow("Glut OpenGL Fraktal");
    glutReshapeFunc(myReshape);
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(myKeyboard);
		
	glutMainLoop();
	return 0;
}

