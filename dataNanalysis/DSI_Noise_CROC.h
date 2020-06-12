/* CROC analysis code for a directory of datafiles. 
Giorgos Papadopoulos, gpapad14@hotmail.com / gpapadop@lpnhe.in2p3.fr
Date: 19 Nov 2019
*/

void DSI_Noise_CROC(TString dir, TString filename, int channelAnalyse, bool saveLvl1, bool saveLvl2, bool mkNewOutputFile, TString outputFileName, double GainSet) {
	gStyle->SetOptFit(1111);	

	int PointsPerMeas = 1;
	int channels = 4;
	int bitsADC = 18;
	int Ncodes = pow(2,bitsADC);
	double Tconv = 15*1e-6; // Sampling/measurement period in [s]. 
	double integrationTime = 1e-6; // in s.

	bool plotRaw=true;
	bool plotHist=true;
	bool DOanalysis=false;

//------------------------------------------------------------------------------------
// Measure the number of measurements
	ifstream inp;
	double x;
	int N=0;
	inp.open(dir+filename);		
	while(inp>>x) {	N++; }
	inp.close();
	//cout<<"$ #measurements="<<N/(PointsPerMeas*channels)<<", "<<PointsPerMeas<<" points/meas"<<endl;

	inp.open(dir+filename);
	int exc = 200;
	for(int ivar=0; ivar<exc*PointsPerMeas*channels; ivar++){ inp>>x; }
	N = N - exc * PointsPerMeas * channels;

// Create raw_graph and its histogram
	TGraphErrors* grRaw = new TGraphErrors(N/(PointsPerMeas*channels));
	int binwidth=8;
	TH1F* histRaw = new TH1F("histRaw", "Histogram of raw data", Ncodes/binwidth, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);
		

	int maxVal = -1*Ncodes;
	int minVal =  Ncodes;
	int Npoints=0;
	TString channelAnalyseSTR = "Ch";
	channelAnalyseSTR+=channelAnalyse;
	//cout<<"$ Channel to analyse: "<<channelAnalyseSTR<<endl;
	for(int i=0; i<N; i++) {
		inp>>x;

		if(i%channels==channelAnalyse) {
			//cout<<i<<endl;
			grRaw->SetPoint(i/channels, i/channels, x);
			//cout<<x<<endl;
			if(x<minVal) { minVal=x; }
			if(x>maxVal) { maxVal=x; }

			histRaw->Fill(x);

			Npoints++;
		}

	}
	inp.close();
	//cout<<"Max = "<<maxVal<<", Min = "<<minVal<<endl;
	//cout<<"$ Ch"<<channelAnalyse<<" Npoints = "<<Npoints << "(before cleaning)" <<endl;

// Plot raw graph
	if(plotRaw) {
		TCanvas* cRaw = new TCanvas("cRaw","cRaw",700,500);
		cRaw->SetGrid();
		cRaw->SetLeftMargin(0.14);
		cRaw->SetRightMargin(0.07);
		grRaw->SetMarkerColor(4);
		grRaw->SetMarkerStyle(8);
		grRaw->GetXaxis()->SetTitle("");
		grRaw->GetYaxis()->SetTitle("ADC out (ADU)");
		grRaw->SetTitle("Raw spectrum of measurement");
		grRaw->Draw("APL");
		//grRaw->Fit("fsin","Q0","");
		if(saveLvl1) { cRaw->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_graph.png",""); }
		delete cRaw;
	}
// Plot histogram of raw/amplitude graph
	
	TCanvas* cHistRaw = new TCanvas("cHistRaw","cHistRaw",700,500);
	cHistRaw->SetGrid();
	histRaw->SetTitle("DSI mode output amplitude");
	histRaw->GetXaxis()->SetRangeUser(minVal, maxVal);
	histRaw->GetXaxis()->SetTitle("ADC out (ADU)");
	histRaw->Draw();
	histRaw->Fit("gaus","Q");
	if(saveLvl1) { cHistRaw->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_hist.png",""); }
	delete cHistRaw;
	TF1* amplfunc = histRaw->GetFunction("gaus");
	double meanAmpl, sigmaAmpl;
	meanAmpl = amplfunc->GetParameter(1);
	sigmaAmpl = amplfunc->GetParameter(2);
	delete histRaw;

	cout<<"   Mean  amplitude  = " << meanAmpl  <<" adu"<<endl;
	cout<<"   Sigma amplitude  = " << sigmaAmpl <<" adu"<<endl;


if(DOanalysis) {
	double ADUbeb = 21.4e-6; 		// in V
	double ADUbebErr=0.1e-6;		// in V
	double inAmplitude = 5.04e-3;	// in V
	double inAmplitudeErr = 0.38e-3;// in V

	// Absolute gain measurement
	double GainSetX = GainSet;
	//cout<<" GainSetX = "<<GainSetX<<endl;
	double par2, par1, par0, par2Err, par1Err, par0Err;
	if(channelAnalyse==0) {
		par2 = -0.023644756;
		par1 =  6.531284974;
		par0 = 15.985265521;
		par2Err = -0.001770;
		par1Err =  0.493063;
		par0Err =  1.213652;
	}
	if(channelAnalyse==1) {
		par2 = -0.021617881;
		par1 =  6.397530169;
		par0 = 16.939866316;
		par2Err = -0.001601;
		par1Err =  0.481688;
		par0Err =  1.302344;
	}
	double abs_gain = par2*GainSetX*GainSetX + par1*GainSetX + par0; 
	double abs_gainErr = par2Err*GainSetX*GainSetX + par1Err*GainSetX + par0Err;
	//cout<<" Absolute gain = " << setprecision(4)<<abs_gain <<" +/- "<<setprecision(3)<<abs_gainErr <<" ("<< setprecision(2)<<100*abs_gainErr/abs_gain <<"%)"<<endl;
	cout<<" Calculate abs_gain = " << setprecision(4)<<abs_gain <<" +/- "<<setprecision(3)<<abs_gainErr <<endl;

	// integration time is defined in the beginning.
	double RCtime = 1e6*inAmplitude*abs_gain*integrationTime/(meanAmpl*ADUbeb); // in us
	double RCtimeErr = RCtime*sqrt( pow(inAmplitudeErr/inAmplitude,2) + pow(abs_gainErr/abs_gain,2) + pow(sigmaAmpl/meanAmpl,2) + pow(ADUbebErr/ADUbeb,2) );
	cout<<"   RC time = "<<setprecision(3)<<RCtime<<" +/- "<<setprecision(2)<<RCtimeErr <<" us "<< setprecision(3)<<100*RCtimeErr/RCtime<<"%"<<endl;
	// The error includes the error of the generator, CROC chip and board, BEB, and oscilloscope. dominating is the error of 
	// the input signal (or the generator) and the oscilloscope resolution which measures the input signal. Both are contributing in the "Error ...".

	//cout<<"Error 1 = "<<pow(inAmplitudeErr/inAmplitude,2)<<endl;
	//cout<<"Error 2 = "<<pow(abs_gainErr/abs_gain,2)<<endl;
	//cout<<"Error 3 = "<<pow(sigmaAmpl/meanAmpl,2)<<endl;
	//cout<<"Error 4 = "<<pow(ADUbebErr/ADUbeb,2)<<endl;
	


// Write output file.
	
	bool errorFound=false;
	//if(abs_gainErr*100/abs_gain <7.55 || abs_gainErr*100/abs_gain >7.57) { errorFound=true; } 
	//cout<<"   Abs gain = " <<setprecision(4)<<abs_gain<<" +/- "<<setprecision(3)<< abs_gainErr<< " ("<< abs_gainErr*100/abs_gain<<"%)" ;
	//if(errorFound) {cout<<"  _ERROR__"<< endl; }
	//else { cout<<""<<endl; }
	if(mkNewOutputFile) {
		ofstream outp(dir+"../"+outputFileName+".txt", std::ios_base::app);
		outp << filename;
		if(errorFound) { outp << " _ERROR__" << std::endl; }
		else { outp<<""<<std::endl;}
		outp << "amp_mean " << meanAmpl<<std::endl;
		outp << "amp_sigm " << setprecision(4) << sigmaAmpl   <<std::endl;
		//outp << "abs_gain " << setprecision(4) << abs_gain    <<std::endl;
		//outp << "abs_gErr " << setprecision(3) << abs_gainErr <<std::endl;
		outp << "RCtconst " << setprecision(3) << RCtime      <<std::endl;
		outp << "RCtmcErr " << setprecision(2) << RCtimeErr   <<std::endl;
		outp << std::endl;
		outp.close();
	}

}
	


}