extrusion.o: extrusion.cpp extrusion.h
main.o: main.cpp /usr/X11R6/include/GL/glut.h /usr/X11R6/include/GL/gl.h \
  /usr/X11R6/include/GL/glext.h /usr/X11R6/include/GL/glu.h extrusion.h \
  quaternion.h
quaternion.o: quaternion.cpp
