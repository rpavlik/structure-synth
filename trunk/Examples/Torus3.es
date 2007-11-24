set maxobjects 160000
{ a 1.0  sat 0.5 } grinder 
set background #fff

rule grinder { 
   36 * { rz 5  rz 2 y 0.1   }   36 * { ry 10  rz 3 z 1.2 b 0.99 h 12  } xbox
} 

rule xbox {
  { s 1.1  } grid
  { b 0.7  color #eee   a 1.0  }  box
}

rule xbox {
 { s 1.1    } grid
 { b 0.7  color #fff  a 1.0    } box
}
