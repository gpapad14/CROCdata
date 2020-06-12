# Written by Giorgos PAPADOPOULOS
# for CROC.v1 data analysis
# Date: March 2020

import numpy as np
import matplotlib.pyplot as plt
import glob as glob
from scipy.stats import norm

def GetFromFilename(filename=''):
	# First, find all "words" in the filename, separated by "_".
	words = []
	word=''
	for letter in filename:
		if letter!='_' :
			word += letter
		else:
			words.append(word)
			word=''
	words.append(word) 
	#print(words)

	parameters = {}
	# Initialize the parameter values. If the parameter is not found in the filename, it will keep this value.
	parameters['gain'], parameters['rc'], parameters['offset'], parameters['inAmp'], \
		parameters['intT'], parameters['pol'], parameters['polib'], parameters['channel'], parameters['mode'] \
		= 0, 0, 120, 5, 1, 127, 127, '0', 'Tr'
	for word in words[:-2]:
		if word=='GP': continue
		if word[0]=='G': parameters['gain']=int(word[1:])			#1: Gain 6-bit [0,63]
		elif word[0:2] == 'RC': parameters['rc'] = int(word[2:])	#2: RC 6-bit [0,63]
		elif word[0] == 'O': parameters['offset'] = int(word[1:])	#3: Offset 8-bit [0,255]
		elif word[0:2]=='In': parameters['inAmp'] = float(word[2:])	#4: Input amplitude in mV
		elif word[0:4]=='intT': parameters['intT'] = word[4:]		#5: integration time in us
		elif word[0:2]=='Ch': parameters['channel']=word[2:]		#6: channel 0,1,2,3
		elif word=='DSI' or word=='Tr': parameters['mode']=word 	#7: mode DSI/Transparent
		elif word[0:5]=='POLIB': parameters['polib']=word[5:]		#8: POLIB 8-bit [0,255]
		# POL must be after POLIB to not confuse the 3 first letters that are common.
		elif word[0:3]=='POL': parameters['pol']=word[3:]			#9: POL 9-bit [0,255]
	parameters['date']=words[-2]									# Date YYYYMMDD
	parameters['time']=words[-1]									# Time HHMMSS
	'''
	print('Gain: ', 	parameters['gain'])
	print('RC: ', 		parameters['rc'])
	print('Offset: ', 	parameters['offset'])
	print('Input ampl: ', 		parameters['inAmp'])
	print('Integration time: ', parameters['intT'])
	print('Channel: ', 	parameters['channel'])
	print('Mode: ', 	parameters['mode'])
	print('POL: ', 		parameters['pol'])
	print('POLIB: ', 	parameters['polib'])
	print('Date: ', 	parameters['date'])
	print('Time: ', 	parameters['time'])
	'''

	# Returns a dictionary with {'parameter_name' : parameter value}
	return parameters

def ReadDatafile(fname):
    f = open(fname)
    data = f.read()
    #print(type(data))
    data = [int(n) for n in data.split()] # One could split the words with another sympol instead "space".
    #print(type(data))
    # Convert the words in an integer array.
    data =  np.asarray(data, dtype=int)
    #print(type(data))
    return data

def ExtractFnameAndSort(filelist, inFolder, extention):
	fnameExt=[]
	timeToSort=[]
	for fname in filelist: 
		f=fname[len(inFolder):-len(extention)]
		fnameExt.append(f)
		timeToSort.append(GetFromFilename(f)['time'])
	fnameExt.sort(key=dict(zip(fnameExt, timeToSort)).get)
	return fnameExt
