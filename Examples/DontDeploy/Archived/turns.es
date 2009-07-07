// seed 61
set maxdepth 10000
set background white
set minsize 0.01 

1 * { s 1  color grey  } r2

rule r {
 forward
}

rule r {
  turn
}


rule r  {
  turn2
}

rule r  {
  turn4
}

rule r  {
  turn3
}

rule r  {
  turn5
}

rule r  {
  turn6
}

rule r  {
  r2
}


rule r2   {
 set seed initial
  r
 { rz 180 s 0.5 } r
}

rule forward md 90  >  r  {
  dbox
{ rz 1 x 0.1 } forward
}


rule turn md 90  >  r  {
    dbox
{ rz 1 x 0.1 } turn
}

rule turn2 md 90  >  r  {
  dbox
{ rz -1 x 0.1 } turn2
}

rule turn3 md 90  >  r  {
    dbox
{ ry -1 x 0.1 } turn3
}

rule turn4 md 90  >  r  {
  dbox
{ ry -1 x 0.1 } turn4
}

rule turn5 md 90  >  r  {
    dbox
{ rx -1 x 0.1 } turn5
}

rule turn6 md 90  >  r  {
  dbox
{ rx -1 x 0.1 } turn6
}

rule dbox {
	{ s 0.2 1 1 } box
}