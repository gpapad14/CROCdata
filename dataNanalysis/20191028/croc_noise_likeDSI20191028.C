/* CROC analysis code for a single datafile. 
Giorgos Papadopoulos, gpapad14@hotmail.com / gpapadop@lpnhe.in2p3.fr
Date: 13 Oct 2019
*/

void croc_noise_likeDSI20191028() {
	gStyle->SetOptFit(1111);

	TString date = "20191028";
	TString filename="G15_O120_ALL_20191028_155352.txt";
	// Choose which channel data you want to see/plot/analyse.
	int channelAnalyse=0;
	TString chN = "0";
	TString chan = "Ch" + chN;
	TString dir = date+"/" ;
	//dir += chan+"/";
	TString folder = "20191028";
	//dir += folder + "/";
	cout<<"Path_to_file: " <<dir<<endl;

	int PointsPerMeas = 6;
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
	bool savecanvas=false;
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

	int firstcut=(8 + PointsPerMeas*300)*channels;
	double excludeEnd = 0.;
	int N = Nraw - firstcut*(PointsPerMeas*channels) - excludeEnd*channels;
	cout<<N/(PointsPerMeas*channels)<<endl;
	

// Create raw_graph and its histogram
	TGraphErrors* grRaw = new TGraphErrors(N/(PointsPerMeas*channels));

	// Paolo's method -> what I was doing when there is input signal with base and top levels.
	double base;
	double top;
	double noisePaolo;
	// Giorgos/Romain's idea -> for every 6p find the average and subtruct it from all 6 points.
	double avr;
	double noiseRomain0;
	double noiseRomain1;
	double noiseRomain2;
	double noiseRomain3;
	double noiseRomain4;
	double noiseRomain5;
	double minP;
	double maxP;
	int ipack6=0;
	int Npack6=0;
	double dum[PointsPerMeas];
	double span;

	TGraph* grBASE = new TGraph(N/(PointsPerMeas*channels));
	TGraph* grTOP = new TGraph(N/(PointsPerMeas*channels));
	TGraph* grNoisePAOLO = new TGraph(N/(PointsPerMeas*channels));
	TGraph* grAVR = new TGraph(N/(PointsPerMeas*channels));
	TGraph* grNoiseRomain = new TGraph(N/(PointsPerMeas*channels));
	TGraph* grMINP = new TGraph(N/(PointsPerMeas*channels));
	TGraph* grMAXP = new TGraph(N/(PointsPerMeas*channels));
	TGraph* grSPAN = new TGraph(N/(PointsPerMeas*channels));

	int binwidth=8;
	TH1F* histRaw = new TH1F("histRaw", "Histogram of raw data", Ncodes/binwidth, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);

	TH1F* histRomain = new TH1F("histRomain", "noise Romain", Ncodes/1, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);
	TH1F* histPaolo = new TH1F("histPaolo", "noise Paolo", Ncodes/1, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);
	TH1F* histAVR = new TH1F("histAVR", "Average of 6 points", Ncodes/4, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);
	TH1F* histSPAN = new TH1F("histSPAN", "Span of 6 points", Ncodes/1, -1*Ncodes/2-0.5, Ncodes/2-1+0.5);
		
	inp.open(dir+filename);

	int maxVal = -1*Ncodes;
	int minVal =  Ncodes;
	int Npoints=0;
	int ivar=0;
	TString channelAnalyseSTR = "Ch";
	channelAnalyseSTR+=channelAnalyse;
	cout<<"$ Channel to analyse: "<<channelAnalyseSTR<<endl;

	//for(int i=0; i<Nraw - excludeEnd*channels; i++) {
	for(int i=0; i<Nraw - excludeEnd*channels; i++) {
		inp>>x;
		//cout<<i<<endl;
		if(i%4==channelAnalyse) {
			if(Npoints>=firstcut) {
				//cout<<i<<endl;
				grRaw->SetPoint(i/4-firstcut, i/4-firstcut, x);
				//cout<<(i/4-firstcut)/PointsPerMeas<<endl;
				//cout<<"x = "<<x<<endl;
				if(x<minVal) { minVal=x; }
				if(x>maxVal) { maxVal=x; }

				histRaw->Fill(x);
				ivar++;


	//########################################################################################
				if(ipack6==0) {
					maxP = -1*Ncodes;
					minP = Ncodes;
				}
				dum[ipack6]=x;	
				ipack6++;
				//cout<<"ipack6 = "<<ipack6<<endl;		
				if(maxP < x) { maxP=x; }
				if(minP > x) { minP=x; }				

				if(ipack6==PointsPerMeas) {
					Npack6=(i/4-firstcut)/PointsPerMeas;
					//cout<<"#"<< Npack6<<endl;
					//cout<<"#"<< (i/4-firstcut)<<endl;
					base = (dum[0] + dum[1] + dum[2])/3;
					top  = (dum[5] + dum[3] + dum[4])/3;
					avr  = (dum[0] + dum[1] + dum[2] + dum[3] + dum[4] + dum[5])/6;
					//cout<<"#"<< (i/4-firstcut)/PointsPerMeas<<" "<<avr[(i/4-firstcut)/PointsPerMeas]<<endl;
					noisePaolo = top - base;

					noiseRomain0 = dum[0] - avr;
					grNoiseRomain->SetPoint((i/4-firstcut)-5, (i/4-firstcut)-5, noiseRomain0);
					histRomain->Fill(noiseRomain0);
					noiseRomain1 = dum[1] - avr;
					grNoiseRomain->SetPoint((i/4-firstcut)-4, (i/4-firstcut)-4, noiseRomain1);
					histRomain->Fill(noiseRomain1);
					noiseRomain2 = dum[2] - avr;
					grNoiseRomain->SetPoint((i/4-firstcut)-3, (i/4-firstcut)-3, noiseRomain2);
					histRomain->Fill(noiseRomain2);
					noiseRomain3 = dum[3] - avr;
					grNoiseRomain->SetPoint((i/4-firstcut)-2, (i/4-firstcut)-2, noiseRomain3);
					histRomain->Fill(noiseRomain3);
					noiseRomain4 = dum[4] - avr;
					grNoiseRomain->SetPoint((i/4-firstcut)-1, (i/4-firstcut)-1, noiseRomain4);
					histRomain->Fill(noiseRomain4);
					noiseRomain5   = dum[5] - avr;
					grNoiseRomain->SetPoint((i/4-firstcut), (i/4-firstcut), noiseRomain5);
					histRomain->Fill(noiseRomain5);
					
					span = maxP - minP;
					//cout<<"span = "<<span[(i/4-firstcut)/PointsPerMeas]<<endl;

					grBASE->SetPoint(Npack6, Npack6, base);
					grTOP->SetPoint(Npack6, Npack6, top);
					grNoisePAOLO->SetPoint(Npack6, Npack6, noisePaolo);
					grAVR->SetPoint(Npack6, Npack6, avr);
					grMINP->SetPoint(Npack6, Npack6, minP);
					grMAXP->SetPoint(Npack6, Npack6, maxP);
					grSPAN->SetPoint(Npack6, Npack6, span);

					histPaolo->Fill(noisePaolo);
					histAVR->Fill(avr);
					histSPAN->Fill(span);
			
					ipack6=0;
					Npack6++;
					
				}

	//########################################################################################
				

			}
			Npoints++;
		}

	}
	//cout<<"Max = "<<maxVal<<", Min = "<<minVal<<endl;
	cout<<"$ Ch"<<channelAnalyse<<" Npoints = "<<Npoints <<endl;
	cout<<"N of meas = "<<ivar/PointsPerMeas<<endl;


// Plot graphs Romain and Paolo, average, span ----------------------
	TCanvas* cGraphs = new TCanvas("cGraphs",filename,900,600);
	cGraphs->Divide(2,2);
	cGraphs->cd(1);
	cGraphs->cd(1)->SetGrid();
	grNoiseRomain->GetXaxis()->SetTitle("samples");
	grNoiseRomain->GetYaxis()->SetTitle("output signal (ADU)");
	grNoiseRomain->SetTitle("noise Romain: Subtract average from all 6 points");
	grNoiseRomain->SetMarkerStyle(7);
	grNoiseRomain->SetMarkerColor(kBlue);
	grNoiseRomain->Draw("AP");
	grNoiseRomain->Fit("pol1");

	cGraphs->cd(2);
	cGraphs->cd(2)->SetGrid();
	grNoisePAOLO->GetXaxis()->SetTitle("measurements");
	grNoisePAOLO->GetYaxis()->SetTitle("output signal (ADU)");
	grNoisePAOLO->SetTitle("noise Paolo: reconstruct the amplitude with 3-base/3-top points");
	grNoisePAOLO->SetMarkerStyle(7);
	grNoisePAOLO->SetMarkerColor(kBlue);
	grNoisePAOLO->Draw("AP");

	cGraphs->cd(3);
	cGraphs->cd(3)->SetGrid();
	grAVR->GetXaxis()->SetTitle("measurements");
	grAVR->GetYaxis()->SetTitle("reference after clamp (ADU)");
	grAVR->SetTitle("Ref after clamp, average of 6points");
	grAVR->SetMarkerStyle(7);
	grAVR->SetMarkerColor(kBlue);
	grAVR->Draw("AP");

	cGraphs->cd(4);
	cGraphs->cd(4)->SetGrid();
	grSPAN->GetXaxis()->SetTitle("measurements");
	grSPAN->GetYaxis()->SetTitle("span of 6points (ADU)");
	grSPAN->SetTitle("Span of 6points");
	grSPAN->SetMarkerStyle(7);
	grSPAN->SetMarkerColor(kBlue);
	grSPAN->Draw("AP");

	cGraphs->SaveAs(dir + channelAnalyseSTR+"/" + filename + "_GRAPHS" + ".png", "");



// Noise plots Romain/Paolo ------------------------------------------
	TCanvas* cNoiseHist = new TCanvas("cNoiseHist",filename,900,600);
	cNoiseHist->Divide(2,1);
	cNoiseHist->cd(1);
	cNoiseHist->cd(1)->SetGrid();
	//cNoiseHist->SetLeftMargin(0.14);
	//cNoiseHist->SetRightMargin(0.07);
	//grNoisePAOLO->SetMarkerStyle(20);
	//grNoisePAOLO->Draw("AP");
	//histRomain->GetXaxis()->SetRangeUser(-300,300);
	histRomain->GetXaxis()->SetTitle("output signal (ADU)");
	histRomain->Draw();
	histRomain->Fit("gaus");
	histRomain->GetXaxis()->SetRangeUser(histRomain->GetFunction("gaus")->GetParameter(2) * (-4),histRomain->GetFunction("gaus")->GetParameter(2) * 4);

	cNoiseHist->cd(2);
	cNoiseHist->cd(2)->SetGrid();
	//histPaolo->GetXaxis()->SetRangeUser(-300,300);
	histPaolo->GetXaxis()->SetTitle("output signal (ADU)");
	histPaolo->Draw();
	histPaolo->Fit("gaus");
	histPaolo->GetXaxis()->SetRangeUser(histPaolo->GetFunction("gaus")->GetParameter(2) * (-4),histPaolo->GetFunction("gaus")->GetParameter(2) * 4);

	cNoiseHist->SaveAs(dir + channelAnalyseSTR+"/" + filename + "_NOISE" + ".png", "");




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



}