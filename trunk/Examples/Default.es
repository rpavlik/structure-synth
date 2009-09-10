set raytracer::ambient-occlusion [100,100,0.9]
set raytracer::shadows true
set raytracer::reflection 0.4
set raytracer::phong [0.4,0.8,1]

set maxdepth 400
R1
R2


rule R1 {
  { x 1  rz 6 ry 2  s 0.99 hue 1 sat 0.99 } R1
  { s 4 } sphere
  { s 2.2 x 2 hue 120  } sphere 
}

rule R2 {
  { x -1  rz 6 ry 2  s 0.99 hue 1 sat 0.99 } R2
  { s 4} sphere
 { s 2.2  x 2 hue 120 a 0.4  } sphere 
} 