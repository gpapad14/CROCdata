import numpy as np
import matplotlib.pyplot as plt
import glob as glob

def readfile(fname):
    f = open(fname)
    data = f.readlines()[1:]
    data =  np.asarray(data,dtype=int)
    return data

#folder = '/Users/gaior/DAMIC/data/elec/giorgos/'
#folder = '/Users/gpapad/Desktop/DAMIC/ADC_DAC/Rapide 4ADC board/SINanalysis/FFT analysis/Ch0/ADCoutText/'
folder = './20191023/Ch0/Bandwidth_onTop_10kHz/Ch0/'
files = glob.glob(folder + '*.txt')
samplingfreq = 10e3 # 10, 50, 200 kHz
sampling = 1./samplingfreq

#print(files)
ivar=1

for f in files:
    print("--- " + str(ivar))
    ivar+=1
    print(f)
    data = readfile(f)
    print("Nsamples = " + str(len(data)/4)) # divide by 4 channels
    procgain = 10*np.log10(len(data)/2)
    print(int(procgain))
    time = np.arange(0,len(data))*sampling
    fft = np.fft.rfft(data)
    fftfreq = np.fft.rfftfreq(len(data),sampling)

    fig =plt.figure()
    fig.suptitle(f[f.rfind('/')+1:])

    # computes the 10*log10 of the power
    y = 10*np.log10(np.abs(fft)**2)
    max = np.max(y[1:])
    print("Max = " + str(int(max)))
    # subtracts the A_dBMAX to have it normalized to 0, or else 0 corresponds to FS
    #FS = 186.5
    #y = y - FS
    # Real FS if input ampl is not the maximum
    VamplIN=315
    max = max + 20*np.log10(131072/VamplIN)
    y = y - max
    newmax = np.max(y[1:])
    print("New Max = " + str(int(newmax)))

    plt.plot(fftfreq/1e3,y, marker=',', linestyle='-')
    plt.grid(True)
    plt.xlabel('freq [kHz]')
    plt.ylabel('relative power [dB]')
    plt.ylim((-165, 5))
    plt.savefig(f+'.png')
    plt.show()

    fitfunc = np.polyfit(fftfreq/1e6, y, 0)
    func = np.poly1d(fitfunc)
    print("Noise Floor = " + str(int(fitfunc)) + ", SNR = " + str(int(fitfunc+procgain)) )
    print()

