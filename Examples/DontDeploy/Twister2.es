// Camera settings. Place these before first rule call.
set translation [0.0896358 -1.76726 -20]
set rotation [0.985717 0.144947 -0.0857551 -0.0263006 0.635428 0.771712 0.166349 -0.758434 0.630164]
set pivot [0 0 0]
set scale 0.293223

set maxdepth 420
#define max1 130 
#define rzstep 2 
#define rxstep 1 
#define symmetry 6
#define _rz 0
set background white


{ s 100 100 1  rz _rz } box


symmetry * { rz 360/symmetry } r1 
rule r1 md max1 > r2  {
sphere
{ rz rzstep rx rxstep  x 0.3  s 0.999 hue 0.1 } r1
}

rule r2 {
  { s 0.9 rz 220 rx 180  } r3 
}

rule r3  md max1  > r1  {
sphere
{ rz rzstep rx -rxstep x 0.3  } r1
}