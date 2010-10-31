#javascript

function pad(number) {
	number = number + ''; // convert to string
 	while (number.length < 3) {
		number = "0" + number;
	}
	return number;
}


Builder.load("../DontDeploy/Twister2.es");
max = 5;
for (i = 0; i <=  max; i+=1) {
	Builder.reset();	
	Builder.setSize(0,100);
	Builder.define("rzstep",( 2.2*(i/max)));
	Builder.define("rxstep",1);
	Builder.render();
   	Builder.raytraceToFile("XPict" + pad(i) + ".png",true);
	Debug.waitForMouseButton();
}

Debug.Info("This will be written to the console.");
