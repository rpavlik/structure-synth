#javascript

function pad(number) {
	number = number + ''; // convert to string
 	while (number.length < 3) {
		number = "0" + number;
	}
	return number;
}


Builder.load("Examples/Twister2.es");
max = 2;
for (i = 0; i <=  max; i+=1) {
	Builder.reset();	
	Builder.setSize(0,100); // raytracer does not support custom sizes yet. It uses screen size.
	Builder.define("rzstep",( 2.2*(i/max)));
	Builder.define("rxstep",1);
	
      Builder.render();
      name = "f:/Test/out" + pad(i);
	Builder.templateRenderToFile("Sunflow-Colored.rendertemplate", name + ".sc",true); 
 //    Builder.execute("%SUNFLOW%/sunflow2.bat", name + ".sc -o " + name + ".png", true);
    Builder.execute('"C:/Program Files/Java/jdk1.6.0_21/bin/java"', '-Xmx1G -server -jar  "%SUNFLOW%/sunflow.jar" ' +  name + ".sc -nogui -o " + name + ".png", true);

 
}

Debug.Info("This will be written to the console.");
