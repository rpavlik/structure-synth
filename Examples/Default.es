set raytracer::ambient-occlusion [100,100,0.9]
set raytracer::shadows false
set raytracer::reflection 0.0
set raytracer::phong [0.4,0.8,1]

set background #fff

set maxdepth 400
3 * { rz 120 } R1
3 * { rz 120 } R2

rule R1 {
{ x 1.3 rx 1.57 rz 6 ry 3 s 0.99 hue 1 sat 0.99 } R1
{ s 4 a 1 color white } sphere
{ s 2.2 x 2 hue 120 a 0.3 color white } sphere 
}

rule R2 {
{ x -1.3 rz 6 ry 3 s 0.99 hue 1 sat 0.99 } R2
{ s 4 a 1 color white } sphere
{ s 2.2 x 2 hue 120 a 0.3 color white } sphere 
} 