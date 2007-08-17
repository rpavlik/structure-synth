spiral

rule spiral w 50 { 
 dot  
{ y 1 rx 2 s 0.995 } spiral
}

rule spiral w 50 { 
dot  
{ y 1 rx -2 s 0.995 } spiral
}


rule spiral w 10 {  
{ ry 20 }  spiral
} 

rule spiral w 10 {  
{ ry -10 }  spiral
} 

rule spiral w 1 {  
spiral
{ rz 10 }  spiral
} 

rule spiral w 1 {  
spiral
{ rz -5 }  spiral
{ s 5 } sphere
} 

rule spiral w 1 {  
{ s 10 } sphere
} 




