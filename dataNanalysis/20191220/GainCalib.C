// CODE FOR DIRECTORY 20191220

void GainCalib() {
	gStyle->SetOptFit(1111);	

	ifstream inp;
	int channelCalib; // 0,1,2,3
	double BEBadu=21.4e-6; // in V
	double BEBaduErr = 0.1e-6;
	bool save=false;
	inp.open("gainlist.txt");
	TString dumm;
	int Npoints=0;
	while(inp>>dumm) {
		Npoints++;
	}
	Npoints=Npoints/2;
	cout<<"Npoints = "<<Npoints<<endl;
	inp.close();
	inp.open("gainlist.txt");

	double GainXaxis[Npoints];
	double GainYaxis[Npoints];
	int ipoint=0;
	double x;
	while(ipoint<Npoints) {
		inp>>x;
		GainXaxis[ipoint]=x;
		inp>>x;
		GainYaxis[ipoint]=x;
		ipoint++;
	}	

	TGraph* gaincalibration = new TGraph(Npoints, GainXaxis, GainYaxis);
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

	gaincalibration->Fit("pol2","","",4,65);
	legend->AddEntry(gaincalibration->GetFunction("pol2"),"pol2 fit + stats","l");

	TF1* fCF1 = new TF1("fCF1","[0]",0,63);
	fCF1->SetParameter(0,GainYaxis[0]);
	fCF1->SetLineColor(kBlue);
	//fCF1->Draw("SAME");

	legend->Draw("SAME");


/*
	cout<<""<<endl;
	cout<<"@ Absolute gain vs Gain(6-bit)"<< endl;
	for(int ivar=0; ivar<Npoints; ivar ++) {
		if(ivar==0) { cout<<"   modeCF1 : "; }
		else { cout<<"   Gain_"<<GainXaxis[ivar]<<" : "; }
		//cout<< setprecision(4)<<GainYaxis[ivar]<<endl;
	}
*/

	gPad->Update();
	TPaveStats *params2 = (TPaveStats*)gaincalibration->FindObject("stats");
	params2->SetX1NDC(0.75);
	params2->SetX2NDC(0.99);
	params2->SetY1NDC(0.2);
	params2->SetY2NDC(0.38);

	if(true) { cCalibration->SaveAs("absGainCalibration20191220.pdf",""); }


}

