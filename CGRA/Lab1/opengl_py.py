from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import sys

ESCAPE = '\033'
window = 0

def InitGL(Width, Height):                              
    glClearColor(0.0, 0.0, 0.0, 0.0)        
    glClearDepth(1.0)                                       
    glDepthFunc(GL_LESS)                            
    glEnable(GL_DEPTH_TEST)                         
    glShadeModel(GL_SMOOTH)                       
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()                                                                              
    gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)

def ReSizeGLScene(Width, Height):
    glViewport(0, 0, Width, Height)         
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)

def DrawGLScene():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()                                       
    glTranslatef(0.0, 0.0, -6.0)
    # YOUR CODE HERE
    glBegin(GL_QUADS)                   # Start drawing a 4 sided polygon
    glColor3f(0.1,0.1,1.0)
    glVertex3f(-1.0, 1.0, 0.0)          # Top Left
    glVertex3f(1.0, 1.0, 0.0)           # Top Right
    glVertex3f(1.0, -1.0, 0.0)          # Bottom Right
    glVertex3f(-1.0, -1.0, 0.0)         # Bottom Left
    glEnd()                             # We are done with the polygon
        
    glutSwapBuffers()

def keyPressed(*args):
    global window
    if args[0] == ESCAPE:
        sys.exit()

def idle():
     glutPostRedisplay()
     
def main():
    global window
    glutInit(sys.argv)

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
    glutInitWindowSize(640, 480)
    glutInitWindowPosition(0, 0)
    window = glutCreateWindow("CG&RA Python")
    
    glutDisplayFunc(DrawGLScene)
    
    glutIdleFunc(DrawGLScene)
    glutReshapeFunc(ReSizeGLScene)
    glutKeyboardFunc(keyPressed)
    InitGL(640, 480)
    glutMainLoop()

# Print message to console, and kick off the main to get it rolling.
print "Hit ESC key to quit."
main()
