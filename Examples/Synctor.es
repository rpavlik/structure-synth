set maxdepth 600

set background #f94

// Camera settings. Place these before first rule call.
set translation [0 0 -20]
set rotation [-0.0423152 0.017662 -0.998948 -0.0209926 0.999607 0.0185629 0.998884 0.021756 -0.0419277]
set pivot [0 0 0]
set scale 0.236512
set raytracer::ambient-occlusion-samples 0
set raytracer::samples 3

{  sat 0.7 color green } start

rule spiral w 100 {
 box
 { y 0.4 rx 5 hue 1 s 0.995 b 0.999 } spiral
}

rule spiral w 100 {
 box
 { y 0.4 rx 5 hue -1 rz -5 s 0.995 b 0.999 } spiral
}

rule spiral w 100 {
 box
 { y 0.4 rx 5 hue 0 rz 5 s 0.995 b 0.995 } spiral
}

rule spiral w 3 {
 start
} 

rule start {
 set seed initial
 { rx 15 } spiral
 { ry 180 h 3 } spiral
}