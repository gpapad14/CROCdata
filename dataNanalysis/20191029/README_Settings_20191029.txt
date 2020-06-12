CROC
Date: 29 Oct 2019

1) NOISE

Setup:
	CROC in CROC hot test board, no input.
	-> CROC to BEB board v2
	-> BEB3
	-> HSMC to FPGA

	# Trigger to FPGA (there is photo):
		frequency = 10 kHz
		square, 
		ampl = 1.3 Vpp, 
		offset=1.4V, 
		Duty=50%, 

	# Software and firmware version 2. TSfile with TS1,TS2-> there are photos for the actual configuration
		Measured with the oscilloscope (pin CNVADC18) sampling period = 1.52 us (there are screenshots from the oscilloscope SCR01,02)

	# All 4 channels are recorded under the same settings at all time.
		Number of samples: TS1=1, TS2=4500
		NEN on
		V0 on
		Transp on
		POL=POLIB=127
		Offset fixed = 120
		Gain=[5...63] step=5 & CF1

2) Input signal, folders Ch0,1,2,3:
	# Generator
		pulse
		input freq = 40 kHz (FPGA trigger has the same frequency)
			-> T=25us
		delay=0.0
		High=80mV
		Low=0V
		width=10us
			-> High for 10 us, Low for 15us => total T=25us

	# Attenuator = 23

	# from generator to attenuator, from attenuator to oscil, split there with BNC-t and finally send the signal to CROC_in

	# input signal measured from oscilloscope: amplitude 5.8004mV +/- 0.1-0.2mV(?)
