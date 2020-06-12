void comparison(){
	double a0,a0Err,a1,a1Err,a2,a2Err; // pol2 fit parameters from 20191220
	double b0,b0Err,b1,b1Err,b2,b2Err; // pol2 fit parameters from 20200113

	a0 = 16.2776;
	a0Err=0.236455;
	a1=6.63376;
	a1Err=0.0168618;
	a2=-0.0236508;
	a2Err=0.000249348;

	b0=17.2744;
	b0Err=0.685246;
	b1=6.50199;
	b1Err=0.044646;
	b2=-0.0219052;
	b2Err=0.000615288;

	TGraphErrors* grCompare = new TGraphErrors(63);
	TGraphErrors* grA = new TGraphErrors(63);
	TGraphErrors* grB = new TGraphErrors(63);
	double Axi, dAxi, Bxi, dBxi, resxi, dresxi;
	for(double xi=1; xi<=63; xi++) {
		Axi = a2*pow(xi,2) + a1*xi + a0;
		Bxi = b2*pow(xi,2) + b1*xi + b0;
		resxi = Axi - Bxi;
		grA->SetPoint(xi-1, xi, Axi);
		grB->SetPoint(xi-1, xi, Bxi);
		grCompare->SetPoint(xi-1, xi, resxi);

		dAxi = sqrt( pow( a2Err*xi,2) + pow( a1Err*xi,2) + pow( a0Err,2) );
		dBxi = sqrt( pow( b2Err*xi,2) + pow( b1Err*xi,2) + pow( b0Err,2) );
		dresxi = sqrt( pow(dAxi,2) + pow(dBxi,2) );
		grA->SetPointError(xi-1, 0.0, dAxi);
		grB->SetPointError(xi-1, 0.0, dBxi);
		grCompare->SetPointError(xi-1, 0.0, dresxi);

		//cout<<"#"<<xi<<"  -----------"<<endl;
		//cout<<"   "<<Axi<<" +/- "<< dAxi<<endl;
		//cout<<"   "<<Bxi<<" +/- "<< dBxi<<endl;
		//cout<<"   "<<resxi<<" +/- "<< dresxi<<endl;
	}
//==================================================================================
		TCanvas* cgraph = new TCanvas("cgraph","Gain calibration comparison", 800,600);
		cgraph->Draw();
		TPad * pad1 = new TPad("pad1" , "pad1" , 0,0 ,1.,0.3) ; 
		pad1->Draw() ;
		//cgraph->cd() ; 
		TPad * pad2 = new TPad("pad2" , "pad2" , 0,0.3 ,1.,1.) ; 
		pad2->SetBottomMargin(0.005);
		pad2->Draw() ;
		//cgraph->cd() ;
		pad2->cd() ;
		pad2->SetGrid();
		pad2->SetRightMargin(0.07);

		grA->SetMarkerStyle(33);
		grA->SetMarkerColor(2);
		grB->SetMarkerStyle(24);
		grB->SetMarkerColor(4);

		grA->SetTitle("Gain calibration comparison 20191220-20200113");
		grA->GetYaxis()->SetTitle("absolute gain");
		grA->GetYaxis()->SetTitleSize(0.045);
		grA->Draw("AP");
		grB->Draw("PSAME");

		TLegend* legend = new TLegend(0.73,0.48,0.93,0.6);
		legend->AddEntry(grA,"pol2 on 20191220","p");
		legend->AddEntry(grB,"pol2 on 20200113","p");
		legend->Draw("SAME");

		pad1->cd() ;
		pad1->SetGrid();
		pad1->SetTopMargin(0.001);
		pad1->SetRightMargin(0.07);
		pad1->SetBottomMargin(0.2);
		
		grCompare->SetTitle("");
		grCompare->SetMarkerStyle(20);
		grCompare->GetXaxis()->SetTitle("gain (6-bit)");

		grCompare->GetXaxis()->SetTitleOffset(0.9);	
		grCompare->GetXaxis()->SetTitleSize(0.09);
		grCompare->GetXaxis()->SetLabelSize(0.09);

		grCompare->GetYaxis()->SetTitleSize(0.09);
		grCompare->GetYaxis()->SetTitle("residuals");
		grCompare->GetYaxis()->SetTitleOffset(0.35);
		grCompare->GetYaxis()->SetLabelSize(0.084);
		
		
		grCompare->Draw("AP");

		//cgraph->SaveAs("gainComparison20191220_20200113.pdf","");
		//cgraph->SaveAs("gainComparison20191220_20200113.png","");
	
}