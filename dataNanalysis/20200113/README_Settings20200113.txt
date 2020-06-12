README_Settings file, Date: 13 Jan 2020

 Setup: Generator -> BNC_to_SMA cable directly to CROC (no split on the oscilloscope)
	CROC -> BEB_to_CROC -> BEB -> FPGA
	FPGA SMAout send the trigger pulse to generator.
	BEB power supply:
		+12V -> 0.186 A
		-5V -> 0.022 A
		+3.3V -> 0.01 A

	Generator: Keysight 33600A waveform generator
		freq set =100kHz -> actual input period 20us = 50kHz
		High lvl = 1, 2, 3, 4, 5, 6, 7, 8, 9 mV
		Low lvl = 0V
		pulse width = 6us
		edge time = 2.9ns
		Burst option

	LabView: CROC_TEST2.vi
	Firmware: ...v2_TS1_TS2_1Mhz
	TS: ..._GP_TrTSfile.txt

	CROC LabView configuration
	Signal to only Ch0, the rest NEN=on, CF1=OFF, Gain=0
	POL=POLIB=127
	Offset=120
	NEN=V1=Transp=ON

	TS1= 2200 samples
	TS2=1
	conv = 6 (6 points per sample)


G20_input11mV -> very bad but only when I save the data!!!
G55_input5mV -> very bad BOTH when save or not!!!