This is a work in progress to create a wireless 2 probe thermometer based on:
- NodeMCU
- MCP3208 8 x 12 Bit Analog to Digital Converter (Has 8 channels but I only need to read 2 probes)
- Maverick ET-732 Probes 

Creates a captive portal on a WIFI network called WiFiThermometer for initial setup. Selecting that network automagically displays the home page which shows the probe temperature. A navigation bar is available with an option to configure WIFI. This allows configuration of your home WIFI SSID and password so it can connect to your network.

So far, I managed to read the probe temperature using two channels of the mcp3208 to about 2 degree of what they should be so some additional calibration will be required to get Steinhart-Hart coefficients right, but it's pretty close.

Out of the box, the ET-732 probes are very flaky. Their values are all over the place. But I added a wire from the probe's metal mesh shielding to the 2.5mm connector which is going to ground and values are very stable now. 

Next:
- I would like to add a battery level monitoring functionality (I'll probably use another of the mpc3208's channels)
- Improve the Web interface layout with a simple framework (maybe bootstrap).
- Ultimately, I want the thermometer to send its data to a cloud web service that would also be used by the client to interact with it.
