/* CROC analysis code for a single datafile. 
Giorgos Papadopoulos, gpapad14@hotmail.com / gpapadop@lpnhe.in2p3.fr
Date: 13 Oct 2019
*/

void croc_noise() {
	gStyle->SetOptFit(1111);

	TString date = "20191028";
	TString filename="G30_O120_ALL_20191028_155511.txt";
	// Choose which channel data you want to see/plot/analyse.
	int channelAnalyse=0;
	TString chN = "0";
	TString chan = "Ch" + chN;
	TString dir = date+"/" ;
	//dir += chan+"/";
	TString folder = "20191028";
	dir += folder + "/";
	//cout<<"Path_to_file: " <<dir<<endl;

	int PointsPerMeas = 1;
	int channels = 4;
	int bitsADC = 18;
	int Ncodes = pow(2,bitsADC);
	double Tconv = 100*1e-6; // Conversion period in [s]. 

	bool plotRaw=true;
	bool plotHist=true;
	bool plotHistRaw2 = true;
	bool saveHistRaw2 = false;
	bool DOanalysis=false;
	bool plotAmplitude=true;
	bool plotHistAmpl=true;
	bool savecanvas=true;
	bool writeBaseFile=true; // code not yet written
	bool writeTopFile=true; // code not yet written


//------------------------------------------------------------------------------------
// Measure the number of measurements
	ifstream inp;
	double x;
	int Nraw=0;
	inp.open(dir+filename);		
	while(inp>>x) {	Nraw++; }
	inp.close();
	//cout<<"$ #measurements="<<N/(PointsPerMeas*channels)<<", "<<PointsPerMeas<<" points/meas"<<endl;

	int firstcut=500;
	double excludeEnd = 0.;
	int N = Nraw - firstcut*(PointsPerMeas*channels) - excludeEnd*channels;
	cout<<N/(PointsPerMeas*channels)<<endl;
	

// Create raw_graph and its histogram
	TGraphErrors* grRaw = new TGraphErrors(N/(PointsPerMeas*channels));
	int binwidth=8;
	TH1F* histRaw = new TH1F("histRaw", "Histogram of raw data", Ncodes/binwidth, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);
		
	inp.open(dir+filename);

	int maxVal = -1*Ncodes;
	int minVal =  Ncodes;
	int Npoints=0;
	int ivar=0;
	TString channelAnalyseSTR = "Ch";
	channelAnalyseSTR+=channelAnalyse;
	cout<<"$ Channel to analyse: "<<channelAnalyseSTR<<endl;

	for(int i=0; i<Nraw - excludeEnd*channels; i++) {
		inp>>x;
		//cout<<i<<endl;
		if(i%4==channelAnalyse) {
			if(Npoints>=firstcut) {
				//cout<<i<<endl;
				grRaw->SetPoint(i/4-firstcut, i/4-firstcut, x);
				//cout<<x<<endl;
				//cout<<i/4-firstcut<<endl;
				if(x<minVal) { minVal=x; }
				if(x>maxVal) { maxVal=x; }

				histRaw->Fill(x);
				ivar++;
			}
			Npoints++;
		}

	}
	//cout<<"Max = "<<maxVal<<", Min = "<<minVal<<endl;
	cout<<"$ Ch"<<channelAnalyse<<" Npoints = "<<Npoints <<endl;
	cout<<"N of meas = "<<ivar/PointsPerMeas<<endl;

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
		grRaw->SetTitle( channelAnalyseSTR+"_"+filename+"_RAW");
		grRaw->Draw("APL");
		//grRaw->Fit("fsin","Q0","");
		if(savecanvas) { cRaw->SaveAs(dir + channelAnalyseSTR+"/" + filename + "_RAW.png",""); }
		//delete cRaw;
	}
/*
	TF1* explinfunc = new TF1("explinfunc", " expo(0) + [2]*x+[3] ", -0.01, 10000);
	explinfunc->SetLineWidth(3);
	explinfunc->SetParameters( -1, 0, -1);
	explinfunc->SetParNames("exp_const", "exp_lambda", "l_slope","l_offset");
	//double RCtime = -1000/expfunc->GetParameter(2);
	//cout<< "RC time = " << RCtime<< " ms"<<endl;
*/

// Plot histogram of raw graph
	if(plotHist) {
		TCanvas* cHistRaw = new TCanvas("cHistRaw","cHistRaw",700,500);
		cHistRaw->SetGrid();
		histRaw->GetXaxis()->SetRangeUser(minVal, maxVal);
		histRaw->GetXaxis()->SetTitle("ADC out (ADU)");
		histRaw->SetTitle( channelAnalyseSTR+"_"+filename+"_HIST");
		histRaw->Draw();
		histRaw->Fit("gaus","Q");

		TF1* amplfunc = histRaw->GetFunction("gaus");
		double meanAmpl = amplfunc->GetParameter(1);
		double sigmaAmpl = amplfunc->GetParameter(2);
		cout<<"$ Mean  amplitude  = " << meanAmpl  <<endl;
		cout<<"$ Sigma amplitude  = " << sigmaAmpl <<endl;
		//if(savecanvas) { cHistRaw->SaveAs(dir + channelAnalyseSTR+"/" + channelAnalyseSTR+"_"+filename + "_HIST.png",""); }
		//if(savecanvas) { cHistRaw->SaveAs(dir + "All/" + filename + "_" +channelAnalyseSTR+ "_HIST.png",""); }
	}
	//grRaw->Fit("explinfunc");

	grRaw->Fit("pol9","Q","");
	TF1* funcpol9 = grRaw->GetFunction("pol9");
	double par0 = funcpol9->GetParameter(0);
	double par1 = funcpol9->GetParameter(1);
	double par2 = funcpol9->GetParameter(2);
	double par3 = funcpol9->GetParameter(3);
	double par4 = funcpol9->GetParameter(4);
	double par5 = funcpol9->GetParameter(5);
	double par6 = funcpol9->GetParameter(6);
	double par7 = funcpol9->GetParameter(7);
	double par8 = funcpol9->GetParameter(8);
	double par9 = funcpol9->GetParameter(9);
	double par10 = funcpol9->GetParameter(10);
	TGraphErrors* grNEW = new TGraphErrors(ivar);
	TH1F* histNEW = new TH1F("histNEW", "Histogram of new data", Ncodes/binwidth, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);
	for(int j=0; j<ivar; j++) {
		double xvar, yvar, fromfit;
		grRaw->GetPoint(j,xvar,yvar);
		fromfit= par0 + par1*pow(xvar,1) + par2*pow(xvar,2) + par3*pow(xvar,3) + par4*pow(xvar,4) + par5*pow(xvar,5) + par6*pow(xvar,6) + par7*pow(xvar,7) + par8*pow(xvar,8) + par9*pow(xvar,9); 
		yvar = yvar-fromfit;
		grNEW->SetPoint(j,xvar,yvar);
		histNEW->Fill(yvar);
	}
		TCanvas* cNEW = new TCanvas("cNEW","cNEW",700,500);
		cNEW->SetGrid();
		//cRaw->SetLeftMargin(0.14);
		//cRaw->SetRightMargin(0.07);
		grNEW->SetMarkerColor(4);
		grNEW->SetMarkerStyle(8);
		grNEW->GetXaxis()->SetTitle("");
		grNEW->GetYaxis()->SetTitle("ADC out (ADU)");
		grNEW->Draw("APL");
		cNEW->SaveAs(dir + channelAnalyseSTR+"/" + filename + "_GRAPHsubRC" + ".png","");


		TCanvas* cHistNEW = new TCanvas("cHistNEW",filename+"_subtractRC",700,500);
		cHistNEW->SetGrid();
		histNEW->GetXaxis()->SetRangeUser(-300, +300);
		histNEW->SetTitle(filename);
		histNEW->Draw();
		histNEW->Fit("gaus");
		cHistNEW->SaveAs(dir + channelAnalyseSTR+"/" + filename + "_HISTsubRC" + ".png","");


//===========================================================
// Start the analysis
//===========================================================
	/*
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
	int exclude = 100; // exclude # measurements = #*PointsPerMeas total points.
	for(int i=exclude*PointsPerMeas; i<(exclude+1)*PointsPerMeas; i++) { 
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
	cout<<"$ #measurements="<<Nmeasurements<<", "<<PointsPerMeas<<" points/meas (after cleaning)"<<endl;


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
		//histAmpl->GetXaxis()->SetRangeUser(0.95*minAmpl, 1.05*maxAmpl);
		histAmpl->GetXaxis()->SetTitle("output amplitude (ADU)");
		histAmpl->Draw();
		histAmpl->Fit("gaus","Q");
		TF1* amplfunc = histAmpl->GetFunction("gaus");
		double meanAmpl = amplfunc->GetParameter(1);
		double sigmaAmpl = amplfunc->GetParameter(2);
		histAmpl->GetXaxis()->SetRangeUser(meanAmpl - 4*sigmaAmpl, meanAmpl + 4*sigmaAmpl);
		cout<<"$ Mean  amplitude  = " << meanAmpl  <<endl;
		cout<<"$ Sigma amplitude  = " << sigmaAmpl <<endl;
		if(savecanvas) { cHistAmpl->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_histAmplitude.png",""); }
	}




} // End of analysis.
*/

}