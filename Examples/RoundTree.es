

set maxdepth 400
1 * { ry 90 x 6 } 10 * { rx 36} R1
{ s 200 200 0.1 } box

rule R1 {
  { x 1 rz -4 ry 6 s 0.99 } R1
  { s 2 } sphere
}

rule R1 {
{ x 1 rz 4 ry 6 s 0.99 } R1
{ s 2 } sphere
}
