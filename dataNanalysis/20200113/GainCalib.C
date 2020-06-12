// CODE FOR DIRECTORY 20200113

void GainCalib() {
	gStyle->SetOptFit(1111);	

	ifstream inp;
	int channelCalib; // 0,1,2,3
	double BEBadu=21.4e-6; // in V
	double BEBaduErr = 0.1e-6;
	bool save=false;

	int Nfiles = 10;
	TString GainValue[Nfiles];
	GainValue[0] = "CF1";
	GainValue[1] = "10";
	GainValue[2] = "15";
	GainValue[3] = "20";
	GainValue[4] = "30";
	GainValue[5] = "40";
	GainValue[6] = "45";
	GainValue[7] = "50";
	GainValue[8] = "55";
	GainValue[9] = "63";

	double GainXaxis[Nfiles];
	GainXaxis[0] = 1;
	GainXaxis[1] = 10;
	GainXaxis[2] = 15;
	GainXaxis[3] = 20;
	GainXaxis[4] = 30;
	GainXaxis[5] = 40;
	GainXaxis[6] = 45;
	GainXaxis[7] = 50;
	GainXaxis[8] = 55;
	GainXaxis[9] = 63;
	double GainYaxis[Nfiles];
	double GainYaxisErr[Nfiles];

	TString channelSTR = "";
	channelSTR += "Ch0_";
	TString gainSTR = "";

	for(int ifile=0; ifile<Nfiles; ifile++) {
	//for(int ifile=0; ifile<3; ifile++) {
		gainSTR = "GAIN";
		if(ifile==0){ gainSTR = "CF1"; }
		else { gainSTR += GainValue[ifile]; }
		TString filename=channelSTR+gainSTR+".txt";
		//cout<<"$   File to analyze: "<<filename<<endl;

		TString dumm;
		inp.open(filename);
		int npoints=0;
		while(inp>>dumm) {
			npoints++;
			//cout<<dumm<<endl;
		}
		//cout<<"$   Number of points: "<<npoints/4<<endl;
		npoints=npoints/4 - 1;
		inp.close();
		inp.open(filename);
		inp>>dumm;
		inp>>dumm;
		inp>>dumm;
		inp>>dumm;
		double inAmpl[npoints];
		double inAmplErr[npoints];
		double outAmpl[npoints];
		double outAmplErr[npoints];
		TGraphErrors* graph = new TGraphErrors(npoints);
		int ipoint=0;
		while(ipoint<npoints) {
			inp>>inAmpl[ipoint];
			inAmpl[ipoint]=inAmpl[ipoint];
			inp>>inAmplErr[ipoint];
			inAmplErr[ipoint]=inAmplErr[ipoint];
			inp>>outAmpl[ipoint];
			outAmpl[ipoint]=outAmpl[ipoint]*BEBadu;
			inp>>outAmplErr[ipoint];
			graph->SetPoint(ipoint, inAmpl[ipoint], outAmpl[ipoint]);
			//graph->SetPointError(ipoint, inAmplErr[ipoint], outAmplErr[ipoint]);
			//cout<<inAmpl[ipoint]<<", "<<outAmpl[ipoint]<<endl;
			ipoint++;
		}
		inp.close();


		TCanvas* cgraph = new TCanvas("cgraph",filename, 800,600);
		cgraph->Draw();
		TPad * pad1 = new TPad("pad1" , "pad1" , 0,0 ,1.,0.3) ; 
		pad1->Draw() ;
		//cgraph->cd() ; 
		TPad * pad2 = new TPad("pad2" , "pad2" , 0,0.3 ,1.,1.) ; 
		pad2->SetBottomMargin(0.02);
		pad2->Draw() ;
		//cgraph->cd() ;
		pad2->cd() ;

		pad2->SetGrid();
		pad2->SetRightMargin(0.07);
		graph->SetTitle(filename);
		graph->GetYaxis()->SetTitle("output amplitude (V)");
		graph->GetXaxis()->SetLabelSize(0);
		//graph->GetYaxis()->SetRangeUser(0, 131000);
		graph->SetMarkerStyle(20);
		graph->Draw("AP");
		TF1* fpol1 = new TF1("fpol1","[0] +x*[1]",0, 0.020);
		fpol1->SetParNames("offset", "absGain");

		graph->Fit("fpol1","Q");

		gPad->Update();
		TPaveStats *params = (TPaveStats*)graph->FindObject("stats");
			params->SetX1NDC(0.69);
			params->SetX2NDC(0.89);
			params->SetY1NDC(0.1);
			params->SetY2NDC(0.3);
		double slope = fpol1->GetParameter(1);
		double offset = fpol1->GetParameter(0);
		GainYaxis[ifile]=slope;

		TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(), gPad->GetUxmax(), gPad->GetUymax(),
				gPad->GetUymin()/BEBadu,gPad->GetUymax()/BEBadu,510,"+L");
	   	axis->SetLineColor(kBlue);
	   	axis->SetLabelColor(kBlue);
	   	axis->SetLabelSize(0.03);
	   	axis->SetTitle("output amplitude (ADU)");
	   	axis->SetTitleSize(0.033);
	   	axis->SetTitleOffset(0.9);
	   	axis->SetTextFont(42);
	   	axis->Draw();

		TGraphErrors* residuals = new TGraphErrors(npoints);
		double res[npoints];
		double reconstructedPoint;
		for(ipoint=0; ipoint<npoints; ipoint++) {
			reconstructedPoint= slope*inAmpl[ipoint] + offset;
			res[ipoint] = outAmpl[ipoint] - reconstructedPoint;
			residuals->SetPoint(ipoint, inAmpl[ipoint], res[ipoint] );
		}


		pad1->cd() ;
		pad1->SetGrid();
		pad1->SetTopMargin(0.01);
		pad1->SetRightMargin(0.07);
		pad1->SetBottomMargin(0.2);
		//pad1->SetLeftMargin(0.14);
		residuals->SetTitle("");
		residuals->SetMarkerStyle(20);
		residuals->GetXaxis()->SetTitle("input amplitude (V)");
		residuals->GetXaxis()->SetTitleSize(0.09);
		residuals->GetXaxis()->SetLabelSize(0.09);

		residuals->GetYaxis()->SetTitleSize(0.09);
		residuals->GetYaxis()->SetTitle("residuals (V)");
		residuals->GetYaxis()->SetTitleOffset(0.4);
		residuals->GetYaxis()->SetLabelSize(0.08);
		
		residuals->Draw("AP");

		if(save) { cgraph->SaveAs("./calibPlots/"+channelSTR+gainSTR+".pdf",""); }
		if(save) { cgraph->SaveAs("./calibPlots/"+channelSTR+gainSTR+".png",""); }
		delete cgraph;


		TCanvas* cgainVSinp = new TCanvas("cgainVSinp", "cgainVSinp", 700,500);
		cgainVSinp->SetGrid();
		TGraph* gain = new TGraph(npoints);
		for(int j=0; j<npoints; j++) {
			double gainDumm= outAmpl[j]/inAmpl[j];
			gain->SetPoint(j, inAmpl[j], gainDumm);
		}
		gain->SetMarkerStyle(24);
		gain->GetXaxis()->SetTitle("input amplitude (V)");
		gain->GetYaxis()->SetTitle("absolute gain");
		gain->SetTitle(gainSTR);
		gain->Draw("AP");
		if(save) { cgainVSinp->SaveAs("./calibPlots/"+channelSTR+gainSTR+"_GainVsIn.pdf",""); }
		if(save) { cgainVSinp->SaveAs("./calibPlots/"+channelSTR+gainSTR+"_GainVsIn.png",""); }
		delete cgainVSinp;
	}

	TGraph* gaincalibration = new TGraph(Nfiles, GainXaxis, GainYaxis);
	TCanvas* cCalibration = new TCanvas("cCalibration","Gain calibration", 700,500);
	cCalibration->SetGrid();
	cCalibration->SetRightMargin(0.05);
	gaincalibration->SetMarkerStyle(20);
	gaincalibration->SetTitle("Absolute gain calibration");
	gaincalibration->GetXaxis()->SetTitle("set gain (6-bit)");
	gaincalibration->GetYaxis()->SetTitle("absGain");
	gaincalibration->Draw("AP");

	TF1* flinear = new TF1("flinear","[0]+[1]*x",0,63);
	//flinear->SetParameter(0,GainYaxis[0]);
	flinear->SetLineColor(kBlue);
	flinear->SetLineStyle(2);
	gaincalibration->Fit("flinear", "Q0","",6,24);
	flinear->Draw("SAME");

	TLegend* legend = new TLegend(0.75,0.38,0.99,0.46);
	legend->AddEntry(flinear,"linear fit on first points","l");

	gaincalibration->Fit("pol2","","",5,65);
	legend->AddEntry(gaincalibration->GetFunction("pol2"),"pol2 fit + stats","l");

	TF1* fCF1 = new TF1("fCF1","[0]",0,63);
	fCF1->SetParameter(0,GainYaxis[0]);
	fCF1->SetLineColor(kBlue);
	//fCF1->Draw("SAME");

	legend->Draw("SAME");



	cout<<""<<endl;
	cout<<"@ Absolute gain vs Gain(6-bit)"<< endl;
	for(int ivar=0; ivar<Nfiles; ivar ++) {
		if(ivar==0) { cout<<"   modeCF1 : "; }
		else { cout<<"   Gain_"<<GainXaxis[ivar]<<" : "; }
		cout<< setprecision(4)<<GainYaxis[ivar]<<endl;
	}


	gPad->Update();
	TPaveStats *params2 = (TPaveStats*)gaincalibration->FindObject("stats");
	params2->SetX1NDC(0.75);
	params2->SetX2NDC(0.99);
	params2->SetY1NDC(0.2);
	params2->SetY2NDC(0.38);

	if(true) { cCalibration->SaveAs("absGainCalibration20200113.pdf",""); }
	if(true) { cCalibration->SaveAs("absGainCalibration20200113.png",""); }


}

