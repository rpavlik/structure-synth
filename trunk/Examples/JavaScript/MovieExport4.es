#javascript

function pad(number) {
	number = number + ''; // convert to string
 	while (number.length < 3) {
		number = "0" + number;
	}
	return number;
}


Builder.load("NouveauMovie.es");
max = 30;
for (i = 0; i <=  max; i+=1) {
       c = i/max;

	Builder.reset();	
	Builder.setSize(0,100);
	Builder.define("_rz",c*360);
	Builder.define("_md",20+c*3000);
	Builder.define("_dofa",0.2+ 0.1*Math.sin(c*3.1415*2));
	//Builder.define("_dofb",0.09);
	Builder.render();
   	Builder.raytraceToFile("N" + pad(i) + ".png",true);
}
