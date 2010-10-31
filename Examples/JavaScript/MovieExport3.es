#javascript

Builder.load("../Nouveau.es");
for (i = 10; i <=  100; i+=10) {
	Builder.reset();	
       Builder.prepend("set raytracer::voxel-steps " + i);
	  Builder.prepend("set raytracer::samples 2");
	  Builder.prepend("set raytracer::progressive false");
	Builder.render();
   	Builder.raytraceToFile("XPict" + i +".png",true);
}

