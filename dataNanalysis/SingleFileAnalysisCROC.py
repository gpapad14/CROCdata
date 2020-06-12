import numpy as np
import matplotlib.pyplot as plt
import glob as glob
from scipy.stats import norm
from SupportFuncs import GetFromFilename, ReadDatafile

# Analyse single file.

dateDir=20200304
directory='./'+str(dateDir)+'/'
folder1='Tint1us'
directory+=folder1 +'/'
extention = '.txt'
if dateDir>=20200304: extention = '.xls'
filename = 'G32_RC16_O120_In5_intT1_Ch0_GP_DSI_20200304_121427'
save=False
plotRaw=True

CROCchannels = 4
ADCbits = 18
Ncodes = 2**18
SamplingPeriod = 10e-6 	# in seconds 
SamplingFreq = 1/SamplingPeriod # in Hz


params = GetFromFilename(filename)
gain=params['gain']
rc=params['rc']
intT=params['intT']
#print(params)
data = ReadDatafile(directory+filename+extention)
#print(data)
cut=CROCchannels*200
dataCh = {'Ch0':data[0+cut::CROCchannels], 'Ch1':data[1+cut::CROCchannels], 'Ch2':data[2+cut::CROCchannels], 'Ch3':data[3+cut::CROCchannels]}
#print(len(dataCh['Ch0']))

if plotRaw:
	plt.plot(np.arange(len(dataCh['Ch0'])),dataCh['Ch0'], marker='o', linestyle='', label='Ch0')
	plt.plot(np.arange(len(dataCh['Ch1'])),dataCh['Ch1'], marker='o', linestyle='', label='Ch1')
	plt.plot(np.arange(len(dataCh['Ch2'])),dataCh['Ch2'], marker='o', linestyle='', label='Ch2')
	plt.plot(np.arange(len(dataCh['Ch3'])),dataCh['Ch3'], marker='o', linestyle='', label='Ch3')
	plt.legend()
	plt.grid(True)
	plt.title(filename)
	plt.xlabel('sample')
	plt.ylabel('ADCout (ADU_BEB)')
	if save:
		#plt.savefig(directory + filename+'.pdf')
		plt.savefig(directory + filename+'.png')
	#plt.show()

dataset=dataCh['Ch'+params['channel']]

mean, sigma = norm.fit(dataset)
meanErr = sigma/np.sqrt(len(dataset))
print('Mean: ' +str(round(mean,2)) +' +/- ' +str(round(meanErr,2)))
print('Sigma: '+str(round(sigma,2)))



