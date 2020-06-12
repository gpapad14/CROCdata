Setup:
	Waveform generator -> oscilloscope

Generator:
	Send pulse signals when triggered by the FPGA (I am using the same trigger as when I take measurements with CROC). 

	Frequency: 100 kHz, but the actual frequency with the Trasnparent TS (_GP_TrTSfile) is 50kHz
	Low level: 0
	High level: 1, 5, 10, 15 mV (depends on the name of the file)
	Pulse Width: 7us
	Edge time: 2.9ns 

Oscilloscope:
	time axis (x-axis) division: 2ms
	amplitude axis (y-axis) division: 500uV (ampl=1mV), 1 (ampl=5mV), 2mV (ampl=10,15mV) 