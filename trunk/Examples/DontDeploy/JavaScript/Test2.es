#javascript


World.clear();
pos = new Vector3(1,1,1);

max = 100

k = 5

function GCD(nums)
{
   if(!nums.length) return 0;
       
	for(var r, a, i = nums.length - 1, GCDNum = nums[i]; i;)
   for(a = nums[--i]; r = a % GCDNum; a = GCDNum, GCDNum = r);
   return GCDNum;
}

for (x = 0; x < k; x++) {
for (y = 0; y < k; y++) {
for (z = 0; z < k; z++) {
       World.setColor(new Vector3(1,1,1),1);
Debug.Info(GCD(x,y,z));
	if (GCD(x,y,z)!=2) World.addSphere2(Vector3(x,y,z),0.2);
}
}
}
