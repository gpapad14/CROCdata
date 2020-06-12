/* CROC analysis code for a single datafile. 
Giorgos Papadopoulos, gpapad14@hotmail.com / gpapadop@lpnhe.in2p3.fr
Date: 13 Oct 2019
*/
#include "TMath.h"
void croc_noise(TString input) {
	gStyle->SetOptFit(1111);

	TString date = "20191113";
	TString filename=input;
	// Choose which channel data you want to see/plot/analyse.
	int channelAnalyse=0;
	TString chN = "0";
	TString chan = "Ch" + chN;
	TString dir = date+"/" ;
	//dir += chan+"/";
	TString folder = "clampone_RCtime/OLD";
	dir += folder + "/";
	//cout<<"Path_to_file: " <<dir<<endl;

	int PointsPerMeas = 6;
	int channels = 4;
	int bitsADC = 18;
	int Ncodes = pow(2,bitsADC);
	double Tconv = 20.52*1e-6; // Conversion period in [s]. 
	double Tsampling = 20.52*1e-6;

	bool plotRaw=true;
	bool plotHist=false;
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

	//int firstcut=80;
	int firstcut = 0;
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

				grRaw->SetPoint(i/4-firstcut, (i/4-firstcut)*Tsampling, x);
				//grRaw->SetPoint(i/4-firstcut, i/4-firstcut, TMath::Log(x));

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

	TF1* expfunc = new TF1("expfunc", " expo(0) + [2] ", -0.01, 10000);
	expfunc->SetLineWidth(3);
	expfunc->SetParameters( 8, 0.5, 10000);
	expfunc->SetParNames("exp_const", "exp_lambda", "l_offset");
	//double RCtime = -1000/expfunc->GetParameter(2);
	//cout<< "RC time = " << RCtime<< " ms"<<endl;


// Plot raw graph
	if(plotRaw) {
		TCanvas* cRaw = new TCanvas("cRaw","cRaw",700,500);
		cRaw->SetGrid();
		cRaw->SetLeftMargin(0.14);
		cRaw->SetRightMargin(0.07);
		//cRaw->SetLogx();
		grRaw->SetMarkerColor(4);
		grRaw->SetMarkerStyle(8);
		grRaw->GetXaxis()->SetTitle("time (s)");
		grRaw->GetYaxis()->SetTitle("ADC out (ADU)");
		grRaw->SetTitle( channelAnalyseSTR+"_"+filename+"_RAW");
		grRaw->Draw("APL");
		//grRaw->Fit("fsin","Q0","");
		//grRaw->Fit("expfunc","Q");
		expfunc->Draw("SAME");
		//grRaw->Fit("pol1");
		if(savecanvas) { cRaw->SaveAs(dir + channelAnalyseSTR+"/" + filename + "_RAW.png",""); }
		//delete cRaw;
	}



// Plot histogram of raw graph
	if(plotHist) {
		TCanvas* cHistRaw = new TCanvas("cHistRaw","cHistRaw",700,500);
		cHistRaw->SetGrid();
		histRaw->GetXaxis()->SetRangeUser(minVal, maxVal);
		histRaw->GetXaxis()->SetTitle("ADC out (ADU)");
		histRaw->SetTitle( channelAnalyseSTR+"_"+filename+"_HIST");
		histRaw->Draw();
		histRaw->Fit("gaus","");

		TF1* amplfunc = histRaw->GetFunction("gaus");
		double meanAmpl = amplfunc->GetParameter(1);
		double sigmaAmpl = amplfunc->GetParameter(2);
		cout<<"$ Mean  amplitude  = " << meanAmpl  <<endl;
		cout<<"$ Sigma amplitude  = " << sigmaAmpl <<endl;
		//if(savecanvas) { cHistRaw->SaveAs(dir + channelAnalyseSTR+"/" + channelAnalyseSTR+"_"+filename + "_HIST.png",""); }
		//if(savecanvas) { cHistRaw->SaveAs(dir + "All/" + filename + "_" +channelAnalyseSTR+ "_HIST.png",""); }
	}
	



	/*
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
	*/

	int cutinhist=500; //points to exlude
	TGraphErrors* grNEW = new TGraphErrors(ivar);
	TH1F* histNEW = new TH1F("histNEW", "Histogram of new data", Ncodes/binwidth, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);
	for(int j=0; j<ivar; j++) {
		double xvar, yvar, fromfit;
		grRaw->GetPoint(j,xvar,yvar);
		//fromfit= par0 + par1*pow(xvar,1) + par2*pow(xvar,2) + par3*pow(xvar,3) + par4*pow(xvar,4) + par5*pow(xvar,5) + par6*pow(xvar,6) + par7*pow(xvar,7) + par8*pow(xvar,8) + par9*pow(xvar,9); 
		fromfit = TMath::Exp(expfunc->GetParameter(0)+expfunc->GetParameter(1)*xvar) +expfunc->GetParameter(2);
		//fromfit = grRaw->GetFunction("pol1")->GetParameter(0) + xvar* grRaw->GetFunction("pol1")->GetParameter(1);
		yvar = yvar-fromfit;
		grNEW->SetPoint(j,xvar,yvar);
		if(j>cutinhist) {
			histNEW->Fill(yvar);
		}
	}
		/*
		TCanvas* cNEW = new TCanvas("cNEW","cNEW",700,500);
		cNEW->SetGrid();
		//cRaw->SetLeftMargin(0.14);
		//cRaw->SetRightMargin(0.07);
		grNEW->SetMarkerColor(4);
		grNEW->SetMarkerStyle(8);
		grNEW->SetTitle("Subtract RC time effect + cut");
		grNEW->GetXaxis()->SetTitle("time (s)");
		grNEW->GetYaxis()->SetTitle("ADC out (ADU)");
		grNEW->Draw("APL");

		double lineYmax = grNEW->GetHistogram()->GetMaximum();
		double lineYmin = grNEW->GetHistogram()->GetMinimum();
		TLine *line = new TLine(cutinhist*Tsampling,lineYmin,cutinhist*Tsampling,lineYmax);
		line->SetLineColor(kRed);
		line->Draw("SAME");

		cNEW->SaveAs(dir + channelAnalyseSTR+"/" + filename + "_GRAPHsubRC" + ".png","");


		TCanvas* cHistNEW = new TCanvas("cHistNEW",filename+"_subtractRC",700,500);
		cHistNEW->SetGrid();
		histNEW->GetXaxis()->SetRangeUser(-300, +300);
		histNEW->SetTitle(filename);
		histNEW->Draw();
		histNEW->Fit("gaus","Q");
		cHistNEW->SaveAs(dir + channelAnalyseSTR+"/" + filename + "_HISTsubRC" + ".png","");
	*/


}