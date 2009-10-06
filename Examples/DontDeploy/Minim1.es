
set raytracer::ambient-occlusion [3,64,100,30]
set raytracer::shadows true
set raytracer::light::reflection 0.4
set raytracer::black::reflection 0.4
set raytracer::phong [0.3,0.8,1]


{ s 100 100 1 color white } box
{ s 1 100 100  color white } box


1 * { x 1 y  -10  }   10 * { y 4}   10 * { x 4  }  tbox

rule tbox md 25  {
   { rx 34 s 1.2 1.2 6  } mbox  
}

rule mbox {
  3 * { y 1} m2box
}

rule m2box {
 { color white }  box 
{ z 0.6  s 1 1 0.1 color orange } box 
{ z 0.6  s 1 1 0.05 color blue } box 
}