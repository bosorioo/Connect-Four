#ifndef _GL_GLUT_INCLUDED
#define _GL_GLUT_INCLUDED

    #define GLUT_DISABLE_ATEXIT_HACK
	#include <iostream>

	#ifdef __APPLE__
	#  include <GLUT/glut.h>
	#else
	#  include <GL/glut.h>
	#endif

#endif
