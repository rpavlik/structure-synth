#define qmx rz -angle hue huedef
#define qx rz angle hue -huedef
#define qmy rx -angle  hue huedef
#define qy rx angle hue -huedef
#define qmz ry angle  hue huedef
#define qz ry -angle hue -huedef
#define angle 6
#define huedef 11

R1

rule R1 maxdepth 3  > c2 {
	{ s 1/3 x -1 y -1 qmx qmy } R1 
	{ s 1/3 x -1 y -1  z -1 qmx qmz qmy } R1 
	{ s 1/3 x -1 y -1  z +1 qmx qmy qz } R1 
	{ s 1/3 x 1 y -1  qx qmy } R1 
	{ s 1/3 x 1 y -1  z -1 qx qmy qmz } R1 
	{ s 1/3 x 1 y -1  z +1 qx qmy qz } R1 
	{ s 1/3  y -1  z -1 qmy qmz }  R1  
	{ s 1/3  y -1  z +1 qmy qz} R1 
	{ s 1/3 x -1 y 1  qmx qy } R1 
	{ s 1/3 x -1 y 1  z -1 qmx qy qmz } R1 
	{ s 1/3 x -1 y 1  z +1 qmx qy qz } R1 
	{ s 1/3 x 1 y 1  qx qy } R1 
	{ s 1/3 x 1 y 1  z -1 qx qy qmz } R1 
	{ s 1/3 x 1 y 1  z +1 qx qy qz } R1 
	{ s 1/3  y 1  z -1  qy qmz } R1  
	{ s 1/3  y 1  z +1 qy qz } R1 
	{ s 1/3 x -1   z -1 qmx qmz  } R1 
	{ s 1/3 x -1   z +1 qmx qz } R1 
	{ s 1/3 x 1    z -1 qx qmz } R1 
	{ s 1/3 x 1    z +1  qx qz } R1 

     { s 1/3 x -1  } c3
	{ s 1/3 x +1  } c3 
 { s 1/3 y -1  } c3
	{ s 1/3 y +1   } c3
 { s 1/3 z -1   } c3
	{ s 1/3 z +1  }  c3
{ s 1/3 z +0  } c3
	  
}

rule c2 {
  { color white }  grid
 { sat 0.99 } box
}

rule c3 {

}




