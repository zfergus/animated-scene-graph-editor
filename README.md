# Animated Scene Graph Editor

**GUI for editing and animating a scene graph.**

Animated Scene Graph Editor is a program designed to allow user	to edit and 
animate a scene graph. Users can create and modify the geometry of the scene 
graph by first selecting a desired geometric primitive (triangle, quadrilateral, 
n-sided polygon, or n-edged polyline) to create. These geometric nodes can then 
be transformed by translating, rotating, and scaling them, and the color can be 
changed using the color selection widget.

After creating the scene graph, users can animate their scene graph by using the 
same transformations in key frames. The animation is, by default, a flip book, but 
if enabled by the user, the transformations and color changes can be linearly 
interpolated creating a smooth animation across all frames. 

The Animated Scene Graph Editor was programmed in C++ using the FLTK library and 
OpenGL. The interface and widgets where designed using FLTK while the display 
window is implement using OpenGL. 
