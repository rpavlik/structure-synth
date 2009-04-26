23 *  { x 3} 23 * { y 3 } plan 

set maxobjects 200000

rule plan w 10  {
  3 *  { x 1 } 3 * { y 1 }  frame2
 { z 1 } plan
}

rule plan { }

#define lz 1.1
#define sz 0.1
#define dz 5

rule frame  {
{ s 0.1 1.1 0.1 x 5  z 5 } box
{ s 0.1 1.1 0.1 x 5  z -5 } box
{ s 0.1 1.1 0.1 x -5  z 5 } box
{ s 0.1 1.1 0.1 x -5  z -5 } box

{ s 1 0.1 0.1 y 5  z 5 } box
{ s 1 0.1 0.1 y 5  z -5 } box
{ s 1 0.1 0.1 y -5  z 5 } box
{ s 1 0.1 0.1 y -5  z -5 } box

{ s 0.1 0.1 1 y 5  x 5 } box
{ s 0.1 0.1 1 y 5  x -5 } box
{ s 0.1 0.1 1 y -5  x 5 } box
{ s 0.1 0.1 1 y -5  x -5 } box

}

rule frame2 {
	//{  s lz lz sz z dz } box
 	{  s sz lz lz  x dz } box
	{  s lz sz lz y dz  } box 
	//{  s lz lz sz z -dz } box
 	{  s sz lz lz  x -dz } box
	{  s lz sz lz y  -dz  } box 
}

rule frame2 {
	{  s lz lz sz z dz } box
 	//{  s sz lz lz  x dz } box
	{  s lz sz lz y dz  } box 
	{  s lz lz sz z -dz } box
 	{  s sz lz lz  x -dz } box
	//{  s lz sz lz y  -dz  } box 
}

rule frame2 {
	{  s lz lz sz z dz } box
 	{  s sz lz lz  x dz } box
	//{  s lz sz lz y dz  } box 
	{  s lz lz sz z -dz } box
 	{  s sz lz lz  x -dz } box
	//{  s lz sz lz y  -dz  } box 
}

rule frame2 {
	//{  s lz lz sz z dz } box
 	{  s sz lz lz  x dz } box
	{  s lz sz lz y dz  } box 
	//{  s lz lz sz z -dz } box
 	{  s sz lz lz  x -dz } box
	{  s lz sz lz y  -dz  } box 
}

rule frame2 {
	{  s lz lz sz z dz } box
 	//{  s sz lz lz  x dz } box
	{  s lz sz lz y dz  } box 
	{  s lz lz sz z -dz } box
 	//{  s sz lz lz  x -dz } box
	{  s lz sz lz y  -dz  } box 
}

rule frame2 {
	//{  s lz lz sz z dz } box
 	{  s sz lz lz  x dz } box
	{  s lz sz lz y dz  } box 
	{  s lz lz sz z -dz } box
 	{  s sz lz lz  x -dz } box
	//{  s lz sz lz y  -dz  } box 
}