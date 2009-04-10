


set background white 
r1

rule r1 md 6  {
	{ s 3 color white } sphere
	5 * { ry 72  rx 72 color white }  r2 
}

rule r2 w 2 {
20 * { x 1 } ryx
1 * { x 20 s 0.7 } R1
}

rule r2 {

}


rule ryx  {
  box 
}