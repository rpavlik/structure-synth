set raytracer::ambient-occlusion [0,50,50]
set raytracer::shadows true
set raytracer::anti-alias 2
set raytracer::reflection 0.0
set raytracer::reflective::reflection 0.0
set raytracer::phong [0.6,0.6,0.3]
set raytracer::reflective::phong [0.6,0.6,0.0]

// Camera settings. Place these before first rule call.
set translation [0 -0.367113 -20]
set rotation [0.963994 0.0575931 -0.259613 0.0514549 0.917418 0.394584 0.260899 -0.393735 0.881422]
set pivot [0 0 0]
set scale 0.408795

set background #fff
#define _md 400
#define _rz 0
#define _zoom 1

set maxdepth _md

{ rz _rz s _zoom } r0

rule r0 {
3 * { rz 120  } R1
3 * { rz 120 } R2
}

rule R1 {
{ x 1.3 rx 1.57 rz 6 ry 3 s 0.99 hue 1 sat 0.99 } R1
{ s 4   } sphere
}

rule R2 {
{ x -1.3 rz 6 ry 3 s 0.99 hue 1 sat 0.99 } R2
{ s 4 a 1   } sphere::reflective
} 