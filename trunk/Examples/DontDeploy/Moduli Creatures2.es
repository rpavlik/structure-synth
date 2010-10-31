// Moduli Creatures. Syntopia 2010.
// This one works best when raytraced in e.g. Sunflow.

// Camera settings. Place these before first rule call.
set translation [0.333462 -7.07568 -20]
set rotation [-0.963396 0.252316 -0.090555 0.256621 0.965708 -0.0394128 0.0775057 -0.0612091 -0.995105]
set pivot [0 0 0]
set scale 0.646003

set raytracer::phong [0.5,0.6,0.1]
set raytracer::shadows false
set background white
50 * { x 1 rz 5 s 0.99 hue 2 } 20 * { y 1 rz 3 hue 1 } d

rule d md 30 {
{ z 1 rx 5 } d
{ s 0.01 1 1.2  } box 
} 

rule d w 0.1 {
 e 
} 

rule e md 5 {
{ z 1 rx 5 } e
{ s 0.01  1 1.2  } box 
}