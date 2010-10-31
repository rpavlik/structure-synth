set maxobjects 60000

 { rz 180 color white} R2

set background white

Rule R1 {
// set seed initial
{ s 2 2 3} box
{ z 2 rx -5 s 0.98} R1 
//R3
}
Rule R2{

{ s 2 2 3} box
{ z -2  rx -5 s 0.984} R2 
{x -2 rz 2 s 0.984 } R3
}
Rule R3 weight 0.1{
}
Rule R3{
{ s 3 2 2} box
{x -2 rz 5 s 0.998}R3
}
Rule R3{
set seed initial
{ s 2 3 2} box
{y 2 rz -5 s 0.98}R3
}
Rule R3{
{ s 2 3 2} box
{y -2 rz -5 s 0.98}R3
}
Rule R3{
{ s 2 3 2} box
{x 2 rz -5 s 0.98}R3
} 