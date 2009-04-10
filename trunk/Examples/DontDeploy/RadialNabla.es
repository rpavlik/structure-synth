// 143#41

set colorpool image:001.jpg
//set colorpool randomhue
set maxdepth 50
#define sym 21
set maxsize 3

sym * { ry 360/sym } S1

set background white 
Rule S1 {
//set seed initial 
 R1
}

Rule R1 { dbox set seed initial { z 0.6 rx 5   }  R1 }
Rule R1 { dbox { z 0.5 rx -90 }  R1 }
Rule R1 { dbox{ z 0.6 rz 90 }  R1 }
Rule R1 { dbox{ z 0.6 rz -90 }  R1 }
Rule R1 weight 0.01 { } 
Rule dbox {  { color random s 2 2  0.5  }  box }
Rule dbox weight 0.5 {   { ry 90 s 0.5  1 1 } R1 }
Rule dbox weight 0.5 { { rx 90 s 0.5 2 1 } R1} 