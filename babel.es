// Write EisenScript code here...
set raytracer::ambient-occlusion-samples 20
//{ color red  } frame
//{ color red  y 2 } frame2
//set raytracer::dof [0.31,0.01] 
set raytracer::samples 2
set perspective-angle 40
60  * { y 1 rx 6  color yellow } r1
60  * { y 1 rx 6  color black  } 1 * { x -0.5 s 0.1 3 20 } box

//1 * { s 0.9 } 60  * { y 1 rx 6  color yellow } r1
//1 * { s 0.8 } 60  * { y 1 rx 6  color yellow } r1

set background white

rule r1 md 5 > r2 {
  { x 1 rz 0.5  z  0.2  blend  red 0.1  } r1
 frame
}

rule r2 md 9 {
  { x 1 rz 0.5  z  0.2  blend  red 0.1  } r2
 frame
}

rule r2 w 0.7 {
}

rule frame w 0.1  {
  { rz 5 } frame
}

rule frame w 0.4 {
  { b 0.7 } frame
}

rule frame w 0.2  {
 { z -0.5 s 1 1  0.1 }  box
}

rule frame  w 0.3  {
  { s 1 0.5 1 y -0.5 } frame
  { s 1 0.5 1  y 0.5 } frame
}

rule frame w 0.3  {
  { s 0.5 1 1 x -0.5 } frame
  { s 0.5 1 1  x 0.5 } frame
}

rule frame {
 { z 0.05 y -0.5 z -0.5 s 1 0.3 0.1 }  box
{ z 0.05 y 0.5 z -0.5 s 1 0.3 0.1 }  box
 
 { z 0.05 x 0.4 y 0 z -0.5  s 0.2 1  1 }  box

// { ry -80  z 0.05 x 0.4 y 0 z -0.5  z 10 s  0.1 0.2 20 }  box
 1 * { x -0.5 y -0.3 z -0.5 s 0.1} arc1
}


rule arc1 md 5 > arc2 {
{ x 1 } arc1  
 dbox
}

rule arc2 md 20 > arc3 {
{ x 0.5  rz 9 } arc2  
 dbox
}

rule arc3 md 6 {
{ x 1 } arc3  
 dbox
}

rule dbox {
{ color black } box
{ z  0.2 y -1 s 1 2 1 } box
{ z  -0.04 y -1 s 0.1 2 1 color black } box
}
