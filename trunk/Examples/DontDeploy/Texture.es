set colorpool list:orange,white,white,white
set maxobjects 5000 
// Camera settings. Place these before first rule call.
set translation [3.79805 1.00986 -20]
set rotation [-0.743262 -0.668907 0.00912711 -0.421624 0.45781 -0.782715 0.519388 -0.585615 -0.622307]
set pivot [0 0 0]
set scale 0.412367
10 * { x 5 } 20 * { y 5  rx 60 rz 13 } blade 
{ s 100 100 1 } box
{ z -20 x 40 s 10 } sphere
rule blade {
{ s 1 10 10 color random } box
}