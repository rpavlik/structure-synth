rule start { 
set maxgenerations 600
spiral

} 

rule spiral w 100 { 
 sphere  
{ y 0.4 rx 0.02 s 0.995 } spiral
}

rule spiral w 1 {  
spiral
{ ry 3.13 }  spiral
} 
