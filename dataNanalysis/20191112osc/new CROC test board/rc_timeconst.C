/* CROC analysis code for a single datafile. 
Giorgos Papadopoulos, gpapad14@hotmail.com / gpapadop@lpnhe.in2p3.fr
Date: 13 Oct 2019
*/

void rc_timeconst() {
	gStyle->SetOptFit(1111);

	TString timefn = "M2_time02.txt";
	ifstream inptime;
	TString voltfn = "M2_v02.txt";
	ifstream inpvolt;
	int N = 125000;

	TString xdummy;
	double t,v;
	// Create raw_graph and its histogram
	TGraph* grRaw = new TGraph(N);

	inptime.open(timefn);
	inpvolt.open(voltfn);


	for(int i=0; i<N; i++) {
		inptime>>t;
		inpvolt>>v;
		//cout<<t<<", "<<v<<endl;
		grRaw->SetPoint(i, t, v);

	}


// Plot raw graph
		TCanvas* cRaw = new TCanvas("cRaw","cRaw",700,500);
		cRaw->SetGrid();
		cRaw->SetLeftMargin(0.12);
		cRaw->SetRightMargin(0.07);
		grRaw->SetMarkerColor(4);
		grRaw->SetMarkerStyle(7);
		grRaw->GetXaxis()->SetTitle("time (s)");
		grRaw->GetYaxis()->SetTitle("CROC out (V)");
		grRaw->SetTitle("");
		grRaw->Draw("APL");
		//grRaw->Fit("fsin","Q0","");
		//if(false) { cRaw->SaveAs(dir + filename + "_"+channelAnalyseSTR + "_graph.png",""); }
		//delete cRaw;

	TF1* expfunc = new TF1("expfunc", "expo(0) + [2]  ", -0.01, 1.5);
	expfunc->SetLineWidth(3);
	expfunc->SetParameters( -1, 0, -1);
	expfunc->SetParNames( "exp_const", "exp_lambda", "offset");

	grRaw->Fit("expfunc","","",0.492, 4);

	double RCtime = -1000/expfunc->GetParameter(2);
	cout<< "RC time = " << RCtime<< " ms"<<endl;




}