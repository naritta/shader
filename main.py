import sys
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

def display():
    glClear(GL_COLOR_BUFFER_BIT)
    glColor3f(1,0,0);

    gluLookAt(1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0)
    #glTranslatef(0.5,0,0)
    glutSolidSphere(1.0,16,16)

    glFlush()

def main():
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_RGBA)
    glutInitWindowSize(300, 300)
    glutCreateWindow("test")
    glutDisplayFunc(display)
    glClearColor(0.0, 0.0, 1.0, 1.0)

    glutMainLoop()

if __name__ == '__main__':
    main()
