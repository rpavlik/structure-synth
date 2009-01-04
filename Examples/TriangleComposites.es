// This example shows how to build
// custom primitives from individual polygons.

5 * { x 2 } 5 * { y 2 hue 20 } House

Rule House {
{ color black s 1.01 } Grid
   Box
  { z 1 sat 0.4 } Pyramid
}

Rule House {
{ color black s 1.01 } Grid
Box

{ z 1 } Bar
}

Rule Pyramid { 
Triangle[0,0,0;1,0,0;0.5,0.5,0.5] 
Triangle[1,0,0;1,1,0;0.5,0.5,0.5] 
Triangle[1,1,0;0,1,0;0.5,0.5,0.5] 
Triangle[0,1,0;0,0,0;0.5,0.5,0.5] 
} 

Rule Bar { 
Triangle[0,0,0;1,0,0;0.5,0,0.5] 
Triangle[1,0,0;1,1,0;0.5,0,0.5] 
Triangle[0.5,0,0.5;1,1,0;0.5,1,0.5] 
Triangle[0,1,0;0.5,1,0.5;1,1,0] 
Triangle[0,0,0;0.5,0,0.5;0,1,0] 
Triangle[0.5,0,0.5;0.5,1,0.5;0,1,0] 
} 