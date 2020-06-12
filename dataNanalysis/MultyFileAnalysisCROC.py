import numpy as np
import matplotlib.pyplot as plt
import glob as glob
from scipy.stats import norm
from scipy.optimize import curve_fit
from SupportFuncs import GetFromFilename, ReadDatafile, ExtractFnameAndSort
import matplotlib
import matplotlib.cm as cm
#import ROOT
#from ROOT import TF1, TGraph, TH1F, TCanvas

def gaussfunc(x, p0=1, p1=0, p2=1):
	return p0 * np.exp(-0.5*((x-p1)/p2)**2)

dateDir=20200304
directory='./'+str(dateDir)+'/'
folder1='Tint2us'
directory+=folder1 +'/'
extention = '.txt'
if dateDir>=20200304: extention = '.xls'

CROCchannels = 4
ADCbits = 18
Ncodes = 2**18
ADUbeb=21.4e-6 # in volts
SamplingPeriod = 10e-6 	# in seconds 
SamplingFreq = 1/SamplingPeriod # in Hz

saveRaw=False
saveFFT=False
saveHist=False
showRaw=False
showFFT=False
showHist=False
checkFFT=False

tree={}
files = glob.glob(directory + '*'+extention)
files = ExtractFnameAndSort(files, directory, extention)
print(len(files), 'to analyse')
#print(*files, sep = "\n")

iplot=0
for filename in files: # if you want only some of the files -> files[:2] first few, files[-3:] last few
	#print(filename)
	params = GetFromFilename(filename)
	#print(params)
	data = ReadDatafile(directory+filename+extention)
	cut = 200 * CROCchannels
	dataCh = {'Ch0':data[0+cut::CROCchannels], 'Ch1':data[1+cut::CROCchannels], 'Ch2':data[2+cut::CROCchannels], 'ch3':data[3+cut::CROCchannels]}
	dataset=dataCh['Ch'+params['channel']]
	timeAxis=SamplingPeriod*np.arange(len(dataset))
	
	if showRaw or saveRaw:
		plt.figure()
		plt.plot(timeAxis, dataset, marker='.', linestyle='') #, label='Ch0'
		plt.legend(['Nsamples='+str(len(dataset))])
		plt.grid(True)
		plt.title(filename)
		plt.xlabel('time (s)')
		plt.ylabel('ADCout (ADU_beb)')
		#plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
		if saveRaw:
			#plt.savefig(directory + filename+'_RAW.pdf')
			plt.savefig(directory + filename+'_RAW.png')
		if showRaw: plt.show()
		plt.close()
	
	mean, std = norm.fit(dataset)
	meanErr = std/np.sqrt(len(dataset))
	#print('#'+str(iplot), 'mean: ', round(mean,2), '±', round(meanErr,2), 'std: ', round(std,2))
	params['mean'], params['meanErr'], params['std'] = round(mean,2), round(meanErr,2), round(std,2)
	tree[iplot]=params

	# Histogram + Gaussian fit PLOT
	binwidth=8
	Nbins= int((max(dataset) - min(dataset))/binwidth) # number of bins of histogram
	if (max(dataset) - min(dataset))%binwidth!=0: Nbins+=1
	NinBin, binCenters, patches = plt.hist(dataset, Nbins, (min(dataset),min(dataset)+Nbins*binwidth), alpha=0.75) # , facecolor='g', label='N='+str(len(dataset))
	# NinBin(list): the number of samples in i_bin
	# binCenters(list): Nbins+1 values in which are defined the Nbins
	#print(binCenters)

	# mean values of the histogram bins
	xbins=np.arange(min(dataset)+binwidth*1/2, min(dataset)+binwidth*(Nbins+1/2), binwidth)
	popt, pcov = curve_fit(gaussfunc,xbins,NinBin,bounds=([0.6*max(NinBin), 0.95*mean, 0.8*std], [1.1*max(NinBin), 1.05*mean, 1.2*std]))
	meanEst, meanEstErr, sigma = popt[1], popt[2]/np.sqrt(len(dataset)), popt[2]
	#print('#'+str(iplot), 'mEst: ', round(meanEst,2), '±', round(meanEstErr,2), 'sigma: ', round(sigma,2))
	#print('#'+str(iplot), 'Δmean: ', round(mean-meanEst,2), 'Δsigma: ', round(std-sigma,2))

	
	if showHist or saveHist:
		#plt.plot(xbins,NinBin,marker='.', linestyle='-')
		xcalc = np.arange(min(dataset), max(dataset),1)
		ycalc = gaussfunc(xcalc,popt[0],mean,std)
		plt.plot(xcalc,ycalc,marker=',', linestyle='-', label='calc: mean=%s , std=%s' % (round(mean,1), round(std,1)))
		yestim = gaussfunc(xcalc,popt[0],popt[1],popt[2])
		plt.plot(xcalc,yestim,marker=',', linestyle='-', label='fit: μ=%s , σ=%s' % (round(meanEst,1), round(sigma,1)))
		plt.legend()
		plt.grid(True)
		plt.title('Hist: '+filename)
		plt.xlabel('ADCout (ADU_beb)')
		#plt.ylabel('#')
		if showHist: plt.show()
		if saveHist: plt.savefig(directory + filename+'_HIST.png')
	plt.close()

	# FFT plot
	# We also need the sampling period (SamplingPeriod) and the list of the time information (timeAxis)
	if showFFT or saveFFT or checkFFT:
		fftData = np.fft.rfft(dataset)
		fftfreq = np.fft.rfftfreq(len(dataset),SamplingPeriod)
		fftdB = 10*np.log10(np.abs(fftData)**2)
		noiseFloor = np.mean(fftdB[1:])
		#print(noiseFloor)
		fftdB = fftdB - noiseFloor
		plt.figure()
		#plt.plot(timeAxis,dataset, marker=',', linestyle='-')
		#fftplot= plt.figure()
		#fig.suptitle(f[f.rfind('/')+1:])
		plt.plot(fftfreq/1e3,fftdB, marker=',', linestyle='-', color='red')
		plt.grid(True)
		plt.title('FFT: '+filename)
		plt.xlabel('freq [kHz]')
		plt.ylabel('relative power [dB]')

		if saveFFT: plt.savefig(directory + filename + '_FFT.png')
		elif checkFFT:
			for xfft in fftdB[1:]:
				if xfft>0.25*max(fftdB):
					plt.savefig(directory + filename + '_FFT.png')
					print('#'+str(iplot)+' FFT: '+filename)
		if showFFT: plt.show()
		plt.close()

	iplot+=1	
# ===== End of loop over all files in directory =====

# parameters of interest: gain, rc, intT
# of course we need as well: mean +/- Delta mean
norm = matplotlib.colors.Normalize(vmin=0, vmax=63, clip=True)
mapper = cm.ScalarMappable(norm=norm, cmap='viridis')
fig, ax = plt.subplots()
gainVar, rcVar, meanVar, dmeanVar, intTVar = [], [], [], [], []
iset=0
for i in range(len(tree)):
	#if tree[i]['gain'] == tree[i-1]['gain'] :
	#	gainVar[iset].append(tree[i]['gain'])
	gainVar.append(tree[i]['gain'])
	rcVar.append(tree[i]['rc'])
	meanVar.append(tree[i]['mean'])
	dmeanVar.append(tree[i]['meanErr'])
	intTVar.append(tree[i]['intT'])
	#plt.scatter(tree[i]['rc'],tree[i]['mean'],c=tree[i]['gain'],edgecolors='none')

'''
print(gainVar)
print(rcVar)
print(meanVar)
print(dmeanVar)
print(intTVar)
'''
#sc = plt.scatter(rcVar,meanVar, c=gainVar)
#clb = plt.colorbar(sc)

norm = matplotlib.colors.PowerNorm(gamma=0.5,vmin=min(gainVar), vmax=max(gainVar), clip=True)
mapper = cm.ScalarMappable(norm=norm, cmap='gist_rainbow')
mapper.get_cmap()
gain_colorScale = np.array([(mapper.to_rgba(v)) for v in gainVar])
for x, y, e, color in zip(rcVar, meanVar, dmeanVar, gain_colorScale):
	plt.plot(x+1, y, 'o', color=color)
	plt.errorbar(x+1, y, e, lw=1, capsize=3, color=color)
'''
print(gain_colorScale)
fig = plt.figure()
plt.scatter(rcVar,meanVar,edgecolors='none',c=gain_colorScale)
plt.errorbar(rcVar, meanVar, yerr=1000, xlolims=True, linestyle='', label='xlolims=True', color=gain_colorScale )
'''
#plt.scatter(rcVar,meanVar,edgecolors='none',c=gainVar)
#plt.colorbar()
plt.title('colorscale Gain(6-bit)')
plt.xscale('log', basex=2)
plt.xlim(left=0.8, right=2**7 )
plt.xlabel('RC (6-bit)')
plt.ylabel('ADC out (ADU_beb)')
plt.grid()
fig.colorbar(cm.ScalarMappable(norm=norm, cmap='gist_rainbow'))
plt.show()

#for ikey,measurement in tree.items():


