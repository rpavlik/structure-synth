
spawn

rule spawn {
  set seed initial 
  r1
}

rule r1 w 0.4 {
   { x 1 } r1
  box
}

rule r1  w 0.01 {
 
}

rule r1 w 0.1 {
   { x 1  ry 90  } r1
  box
}

rule r1 w 0.1 {
   { x 1  ry -45  } r1
  box
}

rule r1 w 0.01 {
   r1 
 { rz 90 s 0.5  hue 40 } spawn
}