//  Produced by Structure Synth V 0.4
//  (http://structuresynth.sf.net/)

set maxdepth 600

set background #f94

{ h 170 sat 0.7 color green } start

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