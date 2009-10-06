
set raytracer::ambient-occlusion [3,64,100,400]
set raytracer::shadows false
set raytracer::light::reflection 0.4
set raytracer::black::reflection 0.4
set raytracer::phong [1,0.8,1]


set maxdepth 10


// { x 9 y 9 s 1 1 90 hue 90 } box
6 * { x 3 } 6  * { y 3 color white }  R1
{ x 30 y 30 s 90 90 1 color white } box

Rule R1 {
   { s 1.5 1.6  0.5  } box
  { z 0.6 rx 5 }  R1
}

Rule R1 {
{ s 1.5 1.6  0.5  } box
{ z 0.6 rx -5 }  R1
}

Rule R1 {
{ s 1.5 1.6  0.5  } box
{ z 0.6 rz 5 }  R1
}

Rule R1 {
{ s 1.5 1.6  0.5  } box
{ z 0.6 rz -5 }  R1
}

Rule R1 weight 0.1 {

}