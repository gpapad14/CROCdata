#include "Transparent_Analysis_CROC.h"
#include "Transp_Noise_Average_CROC.h"
#include "Transp_Noise_SignalLike_CROC.h"
#include "Tr_GainCalib_noErrors_CROC.h"
#include "Tr_GainCalib_noErrors_CROC_2SpM.h"
#include "DSI_Analysis_CROC.h"
#include "DSI_Noise_CROC.h"
#include "GetGain.h"
#include "ScanDirectory.h"
#include "Tr_FixedGainCalib_CROC.h"
#include "GetInputAmplitude.h"

void croc_main_analysis() {

	TString date = "20200304";
	TString dir = date+"/";
	TString folder;

	folder = "Tint1us";
	//folder = "PULSE_INPUT_DSI"; // "PULSE_INPUT_ DSI/Transparent"
	//folder = "NOISE_ALL_Transparent"; // "NOISE_INPUT_ DSI/Transparent"
	dir += folder + "/";
	
	TString chanN = "0";
	TString chan = "Ch"+chanN;
	//dir += chan+"/";

	folder = "Gain63";
	//dir += folder + "/";
	
	cout<<"$ Path_to_file: " <<dir<<endl;
	TString extension = ".txt";
	if(date.Atof()>=20200304) { extension=".xls"; }
	int Nfiles;
	TString* filenameList = ScanDirectory(dir,extension,Nfiles);
	cout<<"$ Number of files found: "<<Nfiles<<endl;

	// Choose which channel data you want to see/plot/analyse.
	int channelAnalyse=0;
	cout<<"$ Channel to analyse: "<<channelAnalyse<<endl;
	bool mkOutputFile = false;
	bool saveLvl1 = false;
	bool saveLvl2 = false;

	// Get the input amplitude
	double inAmplitude = 4.936e-3;	// in V 
	double inAmplitudeErr = 0.012e-3;// in V
	if(folder=="3mV_inA") {
		inAmplitude = 2.965e-3;	// in V 
		inAmplitudeErr = 0.013e-3;// in V
	}
	if(folder=="5mV_inA") {
		inAmplitude = 4.936e-3;	// in V 
		inAmplitudeErr = 0.012e-3;// in V
	}
	if(folder=="7mV_inA") {
		inAmplitude = 6.907e-3;	// in V 
		inAmplitudeErr = 0.012e-3;// in V
	}
	if(folder=="9mV_inA") {
		inAmplitude = 8.881e-3;	// in V 
		inAmplitudeErr = 0.012e-3;// in V
	}
	//cout<<"$ Input amplitude: "<<inAmplitude*1000<<" mV"<<endl;
	
	TString outputFileName = "Ch";
	outputFileName += channelAnalyse;
	//outputFileName += "_5mV_inA";
	outputFileName += "_" +folder;
	if(mkOutputFile) {
		ofstream outp(dir+"../"+outputFileName+".txt", std::ios_base::app);
		//outp << "amp_mean ampl_sigm gain_set abs_gain" <<std::endl;
		outp << "inp_Ampl inp_AmplErr out_Ampl outp_AmplErr" <<std::endl;
	}

	cout<<""<<endl;
	double GainSet;
	double inAmpl, inAmplErr;
	int ifile;
	for(ifile=0; ifile<1; ifile++) {
		TString filename = filenameList[ifile];
		cout<<"#"<<ifile<<": "<<filename<<endl;
		//GetFromFilename();
		GetGain(filename,GainSet);
		GetInputAmplitude(filename, inAmpl, inAmplErr);
		inAmpl=inAmpl/1000;
		inAmplErr=inAmplErr/1000;
		//cout<<"@ input amplitude = "<<inAmpl<<endl;
		//cout<<"Input amplitude = "<< setprecision(5)<< inAmpl << " +/- "<< setprecision(2)<<inAmplErr<< " V (" << 100*inAmplErr/inAmpl<<"%)"<<endl;
		//cout<<GainSet<<endl;
		//Transparent_Analysis_CROC(dir, filename, channelAnalyse, saveLvl1, saveLvl2, mkOutputFile, outputFileName);
		//Tr_GainCalib_noErrors_CROC(dir, filename, channelAnalyse, GainSet, inAmplitude, inAmplitudeErr, saveLvl1, saveLvl2, mkOutputFile, outputFileName);
		//Tr_GainCalib_noErrors_CROC_2SpM(dir, filename, channelAnalyse, GainSet, inAmplitude, inAmplitudeErr, saveLvl1, saveLvl2, mkOutputFile, outputFileName);
		//Tr_FixedGainCalib_CROC(dir, filename, channelAnalyse, GainSet, inAmpl, inAmplErr, saveLvl1, saveLvl2, mkOutputFile, outputFileName);
		//Transparent_Noise_CROC(dir, filename, channelAnalyse, saveLvl1, saveLvl2, mkOutputFile, outputFileName,GainSet);
		//Transp_Noise_Average_CROC(dir, filename, channelAnalyse, saveLvl1, saveLvl2, mkOutputFile, outputFileName,GainSet);
		DSI_Analysis_CROC(dir, filename, channelAnalyse, saveLvl1, saveLvl2, mkOutputFile, outputFileName, GainSet);
		//DSI_Noise_CROC(dir, filename, channelAnalyse, saveLvl1, saveLvl2, mkOutputFile, outputFileName, GainSet);
	}

}