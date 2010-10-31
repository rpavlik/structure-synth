set maxdepth 1200
#define max1 130 (float:0-180)
#define rzstep 2  (float:0-4)
#define rxstep 1  (float:0-4)
#define symmetry 6
set background white


{ s 100 100 1 } box


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