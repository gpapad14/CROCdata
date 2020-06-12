/* CROC analysis code for a single datafile. 
Giorgos Papadopoulos, gpapad14@hotmail.com / gpapadop@lpnhe.in2p3.fr
Date: 13 Oct 2019
*/

void croc_analysis() {
	gStyle->SetOptFit(1111);

	TString filename="G5_O120_Ch1_PulseIn_20191119_114930.txt";
	TString date = "20191119";
	TString dir = date+"/";
	// Choose which channel data you want to see/plot/analyse.
	int channelAnalyse=1;
	TString chN = "0";
	TString chan = "Ch" + chN;
	//dir += chan+"/";
	TString folder = "PULSE_INPUT_Transparent/Ch1";
	dir += folder + "/";
	//cout<<"Path_to_file: " <<dir<<endl;

	int PointsPerMeas = 6;
	int channels = 4;
	int bitsADC = 18;
	int Ncodes = pow(2,bitsADC);
	double Tconv = 20*1e-6; // Conversion period in [s]. 

	bool plotRaw=true;
	bool plotHist=true;
	bool plotHistRaw2 = true;
	bool saveHistRaw2 = false;
	bool DOanalysis=true;
	bool plotAmplitude=true;
	bool plotHistAmpl=true;
	bool savecanvas=false;


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
	int exc = 150;
	for(int ivar=0; ivar<exc*PointsPerMeas*channels; ivar++){
		inp>>x;
	}
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
	cout<<"$ Channel to analyse: "<<channelAnalyseSTR<<endl;
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
	cout<<"$ Ch"<<channelAnalyse<<" Npoints = "<<Npoints << "(before cleaning)" <<endl;

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
		if(true) { cRaw->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_graph.png",""); }
		//delete cRaw;
	}
// Plot histogram of raw graph
	if(plotHist) {
		TCanvas* cHistRaw = new TCanvas("cHistRaw","cHistRaw",700,500);
		cHistRaw->SetGrid();
		histRaw->GetXaxis()->SetRangeUser(minVal, maxVal);
		histRaw->GetXaxis()->SetTitle("ADC out (ADU)");
		histRaw->Draw();
		if(false) { cHistRaw->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_hist.png",""); }
	}


//===========================================================
// Start the analysis
//===========================================================
if(DOanalysis) { // Start of analysis

// Find the cut value.
	double meanzeros, sumzeros;
	int nzeros=0;
	int binval;
	for(int ibin=minVal/binwidth; ibin<=maxVal/binwidth; ibin++) {
		binval = histRaw->GetBinContent(ibin+Ncodes/binwidth/2+1);
		//cout<<ibin<<", "<<binval<<endl;
		if(binval==0 && ibin<( maxVal/binwidth-(maxVal-minVal)/binwidth*0.1) && ibin>( minVal/binwidth+(maxVal-minVal)/binwidth*0.1)) {
			sumzeros+=ibin;
			//cout<<ibin<<", "<<sumzeros<<endl;
			nzeros++;
		}
	} 
	//cout<<"Nzeros = "<<nzeros<<endl;
	//cout<<"Find cut in range = ["<<histRaw->GetBinCenter(minVal/binwidth)<<", "<<histRaw->GetBinCenter(maxVal/binwidth-(maxVal-minVal)/binwidth*0.2)<<"]"<<endl;
	meanzeros=sumzeros/nzeros*binwidth;
	cout<<"$ Cut value = "<<meanzeros<<endl;
	double cutval=meanzeros;


// Plot base and plateau gaussian distributions separately.
	TH1F* basePeak = ( TH1F* )histRaw->Clone("basePeak");
	TH1F* platPeak = ( TH1F* )histRaw->Clone("platPeak");
	double meanBase, sigmaBase, meanPlat, sigmaPlat;
	if(plotHistRaw2) {
		TCanvas* cHistRaw2 = new TCanvas("cHistRaw2","Histogram of raw data", 700, 500);
		cHistRaw2->Divide(2,1,0.001,0.001);

		cHistRaw2->cd(1);
		cHistRaw2->cd(1)->SetGrid();
		basePeak->SetTitle("Base level");
		basePeak->Draw("");
		basePeak->Fit("gaus","Q","",minVal, cutval);
		TF1 *gausBase = basePeak->GetFunction("gaus");
		meanBase = gausBase->GetParameter(1);
		sigmaBase = gausBase->GetParameter(2);
		basePeak->GetXaxis()->SetRangeUser(meanBase - 4*sigmaBase, meanBase + 4*sigmaBase);

		cHistRaw2->cd(2);
		cHistRaw2->cd(2)->SetGrid();
		platPeak->SetTitle("Top level");
		platPeak->Draw();
		platPeak->Fit("gaus","Q","",cutval, maxVal);
		TF1 *gausPlat = platPeak->GetFunction("gaus");
		meanPlat = gausPlat->GetParameter(1);
		sigmaPlat = gausPlat->GetParameter(2);
		platPeak->GetXaxis()->SetRangeUser(meanPlat - 4*sigmaPlat, meanPlat + 4*sigmaPlat);
		if(saveHistRaw2) { cHistRaw2->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_BaseTop.png",""); }
	}


// Make a clean-er dataset.
	bool found=false;
	int startpoint;
	int exclude = 0; // exclude # measurements = #*PointsPerMeas total points.
	for(int i=exclude*PointsPerMeas; i<(exclude+1)*PointsPerMeas; i++) { 
		double xcoord,ycoord1,ycoord2,ycoord3;
		grRaw->GetPoint(i,xcoord, ycoord1);
		grRaw->GetPoint(i+1,xcoord, ycoord2);
		grRaw->GetPoint(i+2,xcoord, ycoord3);
		//cout<<"#"<<i<<" "<<ycoord1<<", "<<ycoord2<<", "<<ycoord3<<endl;
		if(ycoord1>cutval && ycoord2>cutval && ycoord3>cutval && !found) {
			found=true;
			startpoint=i-3;
		}
	}
	//cout<<"$ Start point = "<<startpoint<<endl;

	int Nmeasurements = (Npoints-startpoint+1)/PointsPerMeas;
	TGraphErrors* grAmpl = new TGraphErrors(Nmeasurements);

	int binwidthHistAmpl=16;
	TH1F* histAmpl = new TH1F("histAmpl", "Histogram of grAmplitude", (Ncodes/2 - 0)/binwidthHistAmpl, 0, Ncodes/2);
	int maxAmpl = -1*Ncodes;
	int minAmpl =  Ncodes;
	double base[Npoints/PointsPerMeas];
	double top[Npoints/PointsPerMeas];
	double amplitude[Npoints/PointsPerMeas];
	double basedum, topdum;
	int k=0;
	int j;
	for(j=startpoint; j<=Npoints-PointsPerMeas+1; j+=PointsPerMeas){	
		double p1, p2, p3, p4, p5, p6; // The 6 points in every measurement
		double xdum;
		grRaw->GetPoint(j,xdum,p1);
		grRaw->GetPoint(j+1,xdum,p2);
		grRaw->GetPoint(j+2,xdum,p3);
		grRaw->GetPoint(j+3,xdum,p4);
		grRaw->GetPoint(j+4,xdum,p5);
		grRaw->GetPoint(j+5,xdum,p6);
		base[k] = ( p1+p2+p3 )/3;
		top[k] = ( p4+p5+p6 )/3;
		amplitude[k]= top[k] - base[k];
		//cout<<"Raw #"<<j<<", time = "<<k*Tconv<<", ampl = "<<amplitude[k]<<endl;
		grAmpl->SetPoint(k, k*Tconv, amplitude[k]);
		if(amplitude[k]<minAmpl) { minAmpl=amplitude[k]; }
		if(amplitude[k]>maxAmpl) { maxAmpl=amplitude[k]; }
		histAmpl->Fill(amplitude[k]);

		k++;
		//cout<<"j = "<<j<<", "<<k<<endl;
	}
	if(maxAmpl>Ncodes/2) { cout<<"ERROR ! ! ! Increase the histAmpl x-axis range."<<endl; }
	//cout<<"Must: "<<j<<" > "<<Npoints-PointsPerMeas<<endl;
	//cout<<"Must: "<<k<<" = "<<Nmeasurements<<endl;
	// Warning message.
	if(!(j>Npoints-PointsPerMeas) || !(k==Nmeasurements)) {
		//cout<<"Must: "<<j<<" > "<<Npoints-PointsPerMeas<<endl;
		///cout<<"Must: "<<k<<" = "<<Nmeasurements<<endl;
		//cout<<"ERROR ! ! ! Wrong number of points are analysed."<<endl;
	}
	//cout<<"$ #measurements="<<Nmeasurements<<", "<<PointsPerMeas<<" points/meas (after cleaning)"<<endl;


// Plot amplitude graph and its histogram.
	if(plotAmplitude) {
		TCanvas* cAmpl = new TCanvas("cAmpl","cAmpl",700,500);
		cAmpl->SetGrid();
		cAmpl->SetLeftMargin(0.14);
		cAmpl->SetRightMargin(0.07);
		grAmpl->SetMarkerColor(4);
		grAmpl->SetMarkerStyle(7);
		grAmpl->GetXaxis()->SetTitle("time [s]");
		grAmpl->GetYaxis()->SetTitle("output amplitude (ADU)");
		grAmpl->SetTitle("Output pulse amplitude vs time");
		grAmpl->Draw("APL");
		//grRaw->Fit("fsin","Q0","");
		if(true) { cAmpl->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_grAmplitude.png",""); }
		//delete cRaw;
	}

	double meanAmpl, sigmaAmpl;
	//if(plotHistAmpl) {
		TCanvas* cHistAmpl = new TCanvas("cHistAmpl","cHistAmpl",700,500);
		cHistAmpl->SetGrid();
		//histAmpl->GetXaxis()->SetRangeUser(0.95*minAmpl, 1.05*maxAmpl);
		histAmpl->GetXaxis()->SetTitle("output amplitude (ADU)");
		histAmpl->Draw();
		histAmpl->Fit("gaus","Q");
		TF1* amplfunc = histAmpl->GetFunction("gaus");
		meanAmpl = amplfunc->GetParameter(1);
		sigmaAmpl = amplfunc->GetParameter(2);
		histAmpl->GetXaxis()->SetRangeUser(meanAmpl - 4*sigmaAmpl, meanAmpl + 4*sigmaAmpl);
		cout<<"$ Mean  amplitude  = " << meanAmpl  <<endl;
		cout<<"$ Sigma amplitude  = " << sigmaAmpl <<endl;
		if(savecanvas) { cHistAmpl->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_histAmplitude.png",""); }
	//}

	double ADUbeb = 21.4e-6; 		// in V
	double ADUbebErr=0.1e-6;		// in V
	double inAmplitude = 5.04e-3;	// in V
	double inAmplitudeErr = 0.38e-3;// in V

	double absgain=meanAmpl*ADUbeb/inAmplitude;
	double absgainErr = sqrt( pow(ADUbeb*sigmaAmpl/inAmplitude,2) + pow(meanAmpl*ADUbebErr/inAmplitude,2) + pow(meanAmpl*ADUbeb*inAmplitudeErr/(inAmplitude*inAmplitude),2) );
	cout<<"Abs gain = " <<setprecision(4)<<absgain<<" +/- "<<setprecision(3)<< absgainErr<< " ("<< absgainErr*100/absgain<<"%)" <<endl;
	// The error includes the error of the generator, CROC chip and board, BEB, and oscilloscope. dominating is the error of 
	// the input signal (or the generator) and the oscilloscope resolution which measures the input signal. Both are contributing in the "Error 3".

	//cout<<"Error 1 = "<<pow(ADUbeb*sigmaAmpl/inAmplitude,2)<<endl;
	//cout<<"Error 2 = "<<pow(meanAmpl*ADUbebErr/inAmplitude,2)<<endl;
	//cout<<"Error 3 = "<<pow(meanAmpl*ADUbeb*inAmplitudeErr/(inAmplitude*inAmplitude),2)<<endl;

/*
	   TF1 *fsin = new TF1("fsin","[0]*sin([1]*x + [2]) +[3]", 0,Ncodes/2);
	   fsin->SetParameters(3*sigmaAmpl, 2*TMath::Pi()/0.00023, 0, meanAmpl);
	   fsin->SetParLimits(0,100,300);
	   fsin->SetParLimits(2,-TMath::Pi(),TMath::Pi());
*/
	   //grAmpl->Fit("fsin");

} // End of analysis.

}