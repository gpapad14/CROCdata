void CalculateAbsGain(int channelAnalyse, double GainSet, double& abs_gain, double& abs_gainErr) {
	double par2, par1, par0, par2Err, par1Err, par0Err;
	if(channelAnalyse==0) {
		if(GainSet==1) { // GainSet=1 corresponds to CF1 option.
			abs_gain    = 179.2;
			abs_gainErr =  13.6;
		}
		else {
			par2 = -0.023644756;
			par1 =  6.531284974;
			par0 = 15.985265521;
			par2Err = -0.001770;
			par1Err =  0.493063;
			par0Err =  1.213652;
			abs_gain = par2*GainSet*GainSet + par1*GainSet + par0; 
			abs_gainErr = par2Err*GainSet*GainSet + par1Err*GainSet + par0Err;
		}
	}
	if(channelAnalyse==1) {
		if(GainSet==1) { // GainSet=1 corresponds to CF1 option.
			abs_gain    = 195.3;
			abs_gainErr =  14.8;
		}
			else {
			par2 = -0.021617881;
			par1 =  6.397530169;
			par0 = 16.939866316;
			par2Err = -0.001601;
			par1Err =  0.481688;
			par0Err =  1.302344;
			abs_gain = par2*GainSet*GainSet + par1*GainSet + par0; 
			abs_gainErr = par2Err*GainSet*GainSet + par1Err*GainSet + par0Err;
		}
	}
	if(channelAnalyse==2) {
		if(GainSet==1) { // GainSet=1 corresponds to CF1 option.
			abs_gain    = 210.8;
			abs_gainErr =  15.9;
		}
			else {
			par2 = -0.020526319;
			par1 =  6.360176661;
			par0 = 17.179163171;
			par2Err = -0.001561;
			par1Err =  0.481117;
			par0Err =  1.304235;
			abs_gain = par2*GainSet*GainSet + par1*GainSet + par0; 
			abs_gainErr = par2Err*GainSet*GainSet + par1Err*GainSet + par0Err;
		}
	}
	if(channelAnalyse==3) {
		if(GainSet==1) { // GainSet=1 corresponds to CF1 option.
			abs_gain    = 1e-4;
			abs_gainErr =  1e-4;
		}
			else {
			par2 = -0.020508563;
			par1 =  6.327145022;
			par0 = 17.018149251;
			par2Err = -0.001577;
			par1Err =  0.479975;
			par0Err =  1.269628;
			abs_gain = par2*GainSet*GainSet + par1*GainSet + par0; 
			abs_gainErr = par2Err*GainSet*GainSet + par1Err*GainSet + par0Err;
		}
	}

}