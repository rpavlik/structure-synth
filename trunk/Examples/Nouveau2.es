// 26
#define shrink s 0.995

set maxdepth 1000
set background #888

6 * {  rz 60 color white } hbox
// { z -1 s 100 100 1 } box

rule hbox { set seed initial  r}
rule r { { rx 90 } forward }
rule r { { rx -90 }forward }
rule r { forward }
rule r { forward }
rule r { turn }
rule r { turn2 }
//rule r { turn4 }
//rule r { turn3 }
//rule r { turn5 }
//rule r { turn6 }

rule forward md 90 > r {
dbox
{ rz 2 x 0.1 shrink } forward
}

rule turn md 90 > r {
dbox
{ rz 2 x 0.1 shrink } turn
}

rule turn2 md 90 > r {
dbox
{ rz -2 x 0.1 shrink } turn2
}

rule turn3 md 90 > r {
dbox
{ ry -2 x 0.1 shrink } turn3
}

rule turn4 md 90 > r {
dbox
{ ry -2 x 0.1 shrink } turn4
}

rule turn5 md 90 > r {
dbox
{ rx -2 x 0.1 shrink } turn5
}

rule turn6 md 90 >  r {
dbox
{ rx -2 x 0.1 shrink } turn6
}

rule dbox {
{ s 0.2 1 1 } box
{ rx 10 s 0.2 1 1 color black } box
}