
params=new Array(1.0,2.0,5.0,7.5,10.0,15.0,20.0,25.0);
delay=2000;

for (var i=0; i<params.length; i++) {
    tools.logText("setting laser to " + params[i]+"\n");
    instrument.setBrightfieldIntensity(0, 0, params[i]);
    tools.sleepMS(  delay);
    tools.logText("running acquisition at " + params[i]+"mA\n");
    instrument.setAcqDescCurrentSample("LEDIntensity2_"+params[i]+"mA");
    acquisition.doAcquisition();
}