This is a work in progress to create a wireless 2 probe thermometer based on:
- NodeMCU
- MCP3208 (Has 8 channels but I only need to read 2 probes)
- Maverick ET-732 Probes 

So far, I managed to read the probe temperature using two channels of the mcp3208 to about 2 degree of what they should be so some additional calibration will be required to get Steinhart-Hart coefficients right, but it's pretty close.

Out of the box, the ET-732 probes are very flaky. Their values are all over the place. But I added a wire from the probe's metal mesh shielding to the 2.5mm connector which is going to ground and values are very stable now. 

For now, it creates its own AdHoc network, so you connect to it by selecting the MonNodeMCU wireless network and accessing page 192.168.4.1 which shows the temperature of both probes.

Next:
- I would like to add a battery level monitoring functionality (I'll probably use another of the mpc3208's channels)
- Want to add configuration to the Web page so it can connect to the network.
- Ultimately, I want the thermometer to send its data to a cloud web service that would also be used by the client to interact with it.