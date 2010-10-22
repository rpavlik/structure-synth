#define shrink  s 1 

set maxdepth 500
set background white 
set maxobjects 10000
set syncrandom true 


set minsize 0.5
set colorpool list:white
s2

rule s2 md 60 {
   { z 0.8  s 1 1 1 rx 6 } s2
start
}


//{   s 500  500  0.1   color #333 } box

rule start {

6  * { rz 60 } hbox

}



rule hbox md 10 {

  r

}


rule r {
 //set seed initial 
 forward
// { s 0.5 rz 180 } forward
}

rule r {
  turn
}


rule r  {
  turn2
}





rule forward md 90  >  r  {
  dbox
{ rz 2 x 0.1 shrink } forward
}


rule turn md 90  >  r  {
    dbox
{ rz 2  x 0.1 shrink } turn
}

rule turn2 md 90  >  r  {
  dbox
{ rz -2  x 0.1 shrink } turn2
}

rule turn3 md 90  >  r  {
    dbox
{ ry -2 x 0.1 shrink } turn3
}

rule turn4 md 90  >  r  {
  dbox
{ ry -2 x 0.1 shrink } turn4
}

rule turn5 md 90  >  r  {
    dbox
{ rx -2 x 0.1 shrink } turn5
}

rule turn6 md 90  >  r  {
  dbox
{ rx -2 x 0.1 shrink } turn6
}

rule dbox {

	{ s 0.2 1 1  color random } box
}