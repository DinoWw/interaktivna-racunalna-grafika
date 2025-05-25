#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>


void reshape( int width, int height );
void display();
void renderScene();
void animate();
void drawSquare();
int kut;

int main( int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(600, 300);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Primjer Animacije");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(20, animate, 0);
    glutMainLoop();

}

void animate(int value) {
    kut ++;
    if(kut >= 360) {
        kut = 0;
    }
    glutPostRedisplay();
    glutTimerFunc(7, animate, 0);
}

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    // crtanje scene
    renderScene();
    glutSwapBuffers();

}

void reshape(int width, int height) {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width-1, 0, height-1, 0, 1);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_MODELVIEW);
}

void drawSquare() {
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f) ;
    glVertex2f(100.0f, 0.0f) ;
    glVertex2f(100.0f, 100.0f) ;
    glVertex2f(0.0f, 100.0f) ;
    glEnd();
}

void renderScene() {
    glPointSize(1);
    glColor3f(1.0f, 0.0f, 0.3f);
    
    glPushMatrix();
    glTranslatef( 150.0f, 160.0f, 0.0f);
    glScalef( 1.5f, 1.5f, 1.0f );
    glRotatef( (float) kut, 0.0f, 0.0f, 1.0f);
    glTranslatef( -50.0f, -50.0f , 0.0f );
    drawSquare();
    glPopMatrix();

    glPushMatrix();
    glTranslatef( 450.0f, 160.0f, 0.0f);
    glScalef( 1.5f, 1.5f, 1.0f );
    glRotatef( -(float) kut, 0.0f, 0.0f, 1.0f);
    // glTranslatef( -50.0f, -50.0f , 0.0f );
    drawSquare();
    glPopMatrix();
}
