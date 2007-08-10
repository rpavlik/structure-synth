rule b maxdepth 3 > c {

   { s 1/3 x -1 y -1  } b 
   { s 1/3 x -1 y -1  z -1 } b 
   { s 1/3 x -1 y -1  z +1 } b 

   { s 1/3 x 1 y -1  } b 
  { s 1/3 x 1 y -1  z -1 } b 
  { s 1/3 x 1 y -1  z +1 } b 

   { s 1/3  y -1  z -1 } b  
 { s 1/3  y -1  z +1 } b 


   { s 1/3 x -1 y 1  } b 
{ s 1/3 x -1 y 1  z -1 } b 
{ s 1/3 x -1 y 1  z +1 } b 

{ s 1/3 x 1 y 1  } b 
{ s 1/3 x 1 y 1  z -1 } b 
{ s 1/3 x 1 y 1  z +1 } b 

{ s 1/3  y 1  z -1 } b  
{ s 1/3  y 1  z +1 } b 


{ s 1/3 x -1   z -1 } b 
{ s 1/3 x -1   z +1 } b 

{ s 1/3 x 1    z -1 } b 
{ s 1/3 x 1    z +1 } b 


   
}

rule c {
 box

}




