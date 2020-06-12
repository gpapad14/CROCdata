/* CROC analysis code. 
Giorgos Papadopoulos, gpapad14@hotmail.com / gpapadop@lpnhe.in2p3.fr
Date: 13 Oct 2019
*/

void croc_analysis() {
	gStyle->SetOptFit(1111);


	TString filename="8_G63_O130_2019-10-11 150948.txt";
	TString chN = "0";
	TString chan = "Ch" + chN;
	TString dir = chan + "/";
	TString folder = "Gain63";
	dir += folder + "/";
	//cout<<"Path_to_file: " <<dir<<endl;

	// Choose which channel data you want to see/plot/analyse.
	int channelAnalyse=0;

	int PointsPerMeas = 6;
	int channels = 4;
	int bitsADC = 18;
	int Ncodes = pow(2,bitsADC);
	double Tconv = 100*1e-6; // Conversion period in [s]. 


	bool plotRaw=true;
	bool plotHist=true;
	bool plotHistRaw2 = true;
	bool DOanalysis=true;
	bool plotAmplitude=true;
	bool plotHistAmpl=true;
	bool savecanvas=false;


// Measure the number of measuremtns
	ifstream inp;
	double x;
	int N=0;
	inp.open(dir+filename);		
	while(inp>>x) {	N++; }
	inp.close();
	cout<<"$ #measurements="<<N/(PointsPerMeas*channels)<<", "<<PointsPerMeas<<" points/meas"<<endl;

// Create raw_graph and its histogram
	TGraphErrors* grRaw = new TGraphErrors(N/(PointsPerMeas*channels));
	int binwidth=8;
	TH1F* histRaw = new TH1F("histRaw", "Histogram of raw data", Ncodes/binwidth, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);
		
	inp.open(dir+filename);

	int maxVal = -1*Ncodes;
	int minVal =  Ncodes;
	int Npoints=0;
	TString channelAnalyseSTR = "Ch";
	channelAnalyseSTR+=channelAnalyse;
	cout<<"$ Channel to analyse: "<<channelAnalyseSTR<<endl;
	for(int i=0; i<N; i++) {
		inp>>x;

		if(i%4==channelAnalyse) {
			//cout<<i<<endl;
			grRaw->SetPoint(i/4, i/4, x);
			//cout<<x<<endl;
			if(x<minVal) { minVal=x; }
			if(x>maxVal) { maxVal=x; }

			histRaw->Fill(x);

			Npoints++;
		}

	}
	//cout<<"Max = "<<maxVal<<", Min = "<<minVal<<endl;
	cout<<"$ Ch"<<channelAnalyse<<" Npoints = "<<Npoints<<endl;

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
		if(savecanvas) { cRaw->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_graph.png",""); }
		//delete cRaw;
	}
// Plot histogram of raw graph
	if(plotHist) {
		TCanvas* cHistRaw = new TCanvas("cHistRaw","cHistRaw",700,500);
		cHistRaw->SetGrid();
		histRaw->GetXaxis()->SetRangeUser(minVal, maxVal);
		histRaw->GetXaxis()->SetTitle("ADC out (ADU)");
		histRaw->Draw();
		if(savecanvas) { cHistRaw->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_hist.png",""); }
	}

// Find the cut value.
	double meanzeros, sumzeros;
	int nzeros=0;
	int binval;
	for(int ibin=minVal/binwidth; ibin<=maxVal/binwidth; ibin++) {
		binval = histRaw->GetBinContent(ibin+Ncodes/binwidth/2+1);
		//cout<<ibin<<", "<<binval<<endl;
		if(binval==0 && ibin<( maxVal/binwidth-(maxVal-minVal)/binwidth*0.2) ) {
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
		basePeak->GetXaxis()->SetRangeUser(meanBase - 5*sigmaBase, meanBase + 5*sigmaBase);

		cHistRaw2->cd(2);
		cHistRaw2->cd(2)->SetGrid();
		platPeak->SetTitle("Top level");
		platPeak->Draw();
		platPeak->Fit("gaus","Q","",cutval, maxVal);
		TF1 *gausPlat = platPeak->GetFunction("gaus");
		meanPlat = gausPlat->GetParameter(1);
		sigmaPlat = gausPlat->GetParameter(2);
		platPeak->GetXaxis()->SetRangeUser(meanPlat - 6*sigmaPlat, meanPlat + 6*sigmaPlat);
		//if(savecanvas) { cHistRaw2->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_BaseTop.png",""); }
	}



//===========================================================
// Start the analysis
//===========================================================
if(DOanalysis) { // Start of analysis

// Make a clean dataset.
	bool found=false;
	int startpoint;
	for(int i=100; i<100+1*PointsPerMeas; i++) { 
		double xcoord,ycoord1,ycoord2,ycoord3;
		grRaw->GetPoint(i,xcoord, ycoord1);
		grRaw->GetPoint(i+1,xcoord, ycoord2);
		grRaw->GetPoint(i+2,xcoord, ycoord3);
		//cout<<"#"<<i<<" "<<ycoord1<<", "<<ycoord2<<", "<<ycoord3<<endl;
		if(ycoord1>cutval && ycoord2>cutval && ycoord3>cutval && !found) {
			found=true;
			startpoint=i-2;
		}
	}
	cout<<"$ Start point = "<<startpoint<<endl;

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
		base[k] = ( p1+p2+p6 )/3;
		top[k] = ( p3+p4+p5 )/3;
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
		cout<<"Must: "<<j<<" > "<<Npoints-PointsPerMeas<<endl;
		cout<<"Must: "<<k<<" = "<<Nmeasurements<<endl;
		cout<<"ERROR ! ! ! Wrong number of points are analysed."<<endl;
	}

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
		grAmpl->Draw("AP");
		//grRaw->Fit("fsin","Q0","");
		if(savecanvas) { cAmpl->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_grAmplitude.png",""); }
		//delete cRaw;
	}

	if(plotHistAmpl) {
		TCanvas* cHistAmpl = new TCanvas("cHistAmpl","cHistAmpl",700,500);
		cHistAmpl->SetGrid();
		histAmpl->GetXaxis()->SetRangeUser(0.9*minAmpl, 1.1*maxAmpl);
		histAmpl->GetXaxis()->SetTitle("output amplitude (ADU)");
		histAmpl->Draw();
		histAmpl->Fit("gaus");
		TF1* playfunc = histAmpl->GetFunction("gaus");
		cout<<"$ Mean  ampl = " << playfunc->GetParameter(1)<<endl;
		cout<<"$ Sigma ampl = " << playfunc->GetParameter(2)<<endl;
		if(savecanvas) { cHistAmpl->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_histAmplitude.png",""); }
	}




} // End of analysis.

}