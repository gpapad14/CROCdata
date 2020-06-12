Date: 23 Oct 2019
Giorgos PAPADOPOULOS, Romain GAIOR

# Every concerns Trasparent mode.
# input signal, when applied, is always: pulse 0V-80mV, Attenuator=23 -> to Oscilloscope and there split to CROCin_Ch0 

F_factor: 
	Recently I calculated the amplification factor from the ADC driver (buffer) of the BEB3 board, for CF1_Off120_POL/POLIB128_V0on: F = 1.790907 (very close to 1.8 that we were expecting)
	In this folder we have the same measurements for different Gain values (Gain=[15,30,45,63]). To calculate the F factor one also needs the CROCout or BEBin amplitude which we measure with probes with the oscilloscope.
	One can find the table for the F values included in the folder.
	# input signal: pulse 0V-80mV, Attenuator=23 -> to Oscilloscope and there split to CROCin_Ch0

Noise_10/50/200kHz: 
	The perpose of these datasets is to apply a fourier transform (FFT) to the noise of the BEBout raw data. No input in any of the 4 channels, so the analysis can be done for all of them under the same configuration. To do so, for every measurement there is only one sampling. There is also photo of the TimeSlice (TS) window. 
	# Sampling frequency (always equal to FPGA trigger freq) = 10, 50, 200* kHz
	  * For the 200kHz we have changed the TS parameters ("fastTS"). There is photo of the TS window.
	# CF1 and Gain=[15, 30, 45, 63]
	# Offset=120, POL=POLIB=128, V0 is on, NEN is on.

NOISEvsGAIN
	# no input to any channel
	We have observed that the noise is decreasing as we increase the gain, gets very low for CF1 which corresponds to Gain=~30.
	From Gain=[5...63] with a step=5, I want to plot the Noise(sigma) as a function of the Gain(6-bit). 
	I could also apply FFT if I want.

Bandwidth_onTop_10kHz
	This dataset is a single measurement on the plateau of the CROCout signal. Romain recorder the data from the oscilloscope also to make some analysis on those. 
	I could apply an FFT but I do not know what I will get.


