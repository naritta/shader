import sys
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

def display():
    glClear(GL_COLOR_BUFFER_BIT)

    glEnableClientState(GL_VERTEX_ARRAY)
    
    #gluLookAt(150.0,100.0,-200.0,0.0,0.0,0.0,0.0,1.0,0.0)
    glutSolidSphere(0.5, 10, 10)

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
