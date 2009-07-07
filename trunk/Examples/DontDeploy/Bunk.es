set maxobjects 10000
t
set colorpool list:orange,white,blue,white
set background white
// 24,41,62
rule t {
{ rx 45  s 10 10  0.1 color random } box
{ x 12 } t

}


rule t {

{ rx 10 } t
{ ry 10 } t

}

rule t {
set seed initial
{ rz 90 x 1} t

}

rule t {

{ rx  9 z 1 } t

}

rule t {

{ ry 2 z 0.1  } t
{ rz 2 y 1 } t

}