Date: 13 Nov 2019 – 20191113

Today, I tested the new CROC test board (“NEW”) which has 2 50-pin connectors to directly connect to BEB. I also did a comparison between the previous board (with the CROC_to_BEB board), which I call “OLD”, and the new one for the RC time constant after the clamp.

Setup:
FPGA→HSMC cable → BEB3 (#1) +external power supply→ NEW / OLD CROC test board

→ FPGA:
	no external trigger from the generator is needed
	the SMA out will provide trigger to the generator for the input signal that will be used. 

→ BEB
# OLD Power supply consumption
	+12V → ~0.190 A
	-5 V → ~0.022 A 
	+3.3 V→0.01 A
# NEW Power supply consumption
	+12 V → ~0.370 A 	This is a real important thing that needs to be noticed.
	-5 V → ~0.022 A
	+3.3 V → 0.02 A

→ Version 2 software and firmware for all the tests

→ LabView:
	* Gain(6-bit) & CF1 are used
	* Transparent mode & DSI are used
	Offset=120
	POL=POLIB=127
	NEN, V0 always ON

→ Generator:	
	Pulse
	Frequency = 50kHz
	Delay = 10.3μs
	Low = 0mV
	High = 50mV
	Width = 8μs
	L 2.50ns, T 2.50ns
	Cycle: 1-Cycle
	Trig Delay 0.0ns
	Slope Positive
	* external trigger from the SMAout of the FPGA
	Attenuator = 25
	Generator_Ch2out → Attenuator in → Attenuator out → directly to CROC in with BNC_to_SMA cable.
	→ This should give a signal of ~2.8mV. Yet, when I measure with the oscilloscope I see a signal of ~5.7mV.

	

Oscilloscope: 
	light blue → CROC out Channel 0
	orange → conversion pulse (width ~720ns)
	purple → SMA out from FPGA

TEST #1: RC time constant with only one clamp in TS1
	Transparent mode
	→ TS1 = 1
	→ TS2 = 15 000
	→ single conversion for each TS2
	→ sampling period = 20.52 μs (SCR03)

	OLD board, gain=15 (SCR02) full time decay
	NEW board, gain=15 (SCR04) full time decay

	There are data with the LabView for gain=15, 63 and CF1 to analyze.

TEST #2: NEW board, input signal, clamp for every TS2
	Transparent mode
	→ TS1=1
	→ TS2=15 000
	→ TS2 has 6 conversions
	I have added a delay of about 100 μs between TS2 measurements
	→ gain=15 
	images from full decay scale to 2 periods: SCR05, 06, 07, 08 


TEST #3: Like test #2 but I have removed the ~100μs delay. 
	Transparent mode
	→ TS1=1
	→ TS2 has 6 conversions
	→ TS2 period = 20.55 μs
	gain=15 → SCR11
	gain=63 → SCR12
	CF1 → SCR13
	@ in gain mode the clamp is visible in the CROC out signal. On the contrary, in CF1 there is nothing.

TEST #4: DSI mode
	Gain=35, RC=6

	CF1 & input signal →SCR14 : RD goes UP, RU goes UP
	Gain & input signal → SCR15: RD goes DOWN, RU goes UP
	→ I do not understand why there is this difference

	CF1 & no input → SCR16: RD up, RU down
	Gain & no input → SCR17: RD down, RU up
	→ Again I do not understand why there is difference
