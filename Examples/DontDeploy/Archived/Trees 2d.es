// This one is a direct translation of one of Chris Coynes CFDG examples:
// http://www.chriscoyne.com/cfdg/page2.php

set maxdepth 150

FOREST

rule FOREST { 
   SEED
   { x -20 } SEED 
   { x -40 } SEED 
}

rule SEED { BRANCH }

rule SEED { { rz 1 }  BRANCH  }
rule SEED { { rz -1 } BRANCH  }
rule SEED { { rz 2 }  BRANCH  }
rule SEED { { rz -2 } BRANCH  }
rule SEED { FORK  }

rule BRANCH { RIGHTBRANCH  }
rule BRANCH { LEFTBRANCH }

rule LEFTBRANCH w 4 { BLOCK { y 0.885 rz 0.1 s 0.99 }  LEFTBRANCH }
rule LEFTBRANCH w 4 { BLOCK { y 0.885 rz 0.2 s 0.99 } LEFTBRANCH   }
rule LEFTBRANCH w 4 { BLOCK { y 0.885 rz 4 s 0.99 }  LEFTBRANCH }
rule LEFTBRANCH { BLOCK FORK }

rule RIGHTBRANCH w 4 { BLOCK { y 0.885 rz -0.1 s 0.99 }  RIGHTBRANCH }
rule RIGHTBRANCH w 4 { BLOCK { y 0.885 rz -0.2 s 0.99 }  RIGHTBRANCH }
rule RIGHTBRANCH w 4 { BLOCK { y 0.885 rz -4 s 0.99 }  RIGHTBRANCH }
rule RIGHTBRANCH { BLOCK FORK }

rule BLOCK {   box }

rule FORK {    BRANCH  { s 0.5 rz 40 }   BRANCH }
rule FORK {    BRANCH  { s 0.5 rz -40 }   BRANCH }
rule FORK {    BRANCH  { s 0.5 rz -20 }   BRANCH }
rule FORK {    { s 0.7 y 0.1 rz -20 } BRANCH  { s 0.7 y 0.1 rz 20 }   BRANCH }
