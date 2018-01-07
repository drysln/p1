========================================================================
    gethurricaneloss Project Overview
========================================================================

gethurricaneloss.cpp
    This is the main application source file.

Other files:
	CalculateLoss.h
	CalculateLoss.cpp
	EconomicLoss.h
	HurricaneZone.h
	LandfallAnnualEvents.h
	Makefile
	
/////////////////////////////////////////////////////////////////////////////
How to build and install:

	Makefile requires two variables:
	BOOST_LIB - location of boost lib include files
	TARGET - target directory for make install
	
	use make:
	make - builds gethurricaneloss.exe
	make install - builds and installs in ${TARGET}
	make clean
	
/////////////////////////////////////////////////////////////////////////////
Other notes:
	run gethurricaneloss (from the target directory) to see the following:
	
	usage: gethurricaneloss [options] florida_landfall_rate florida_mean florida_stddev gulf_landfall_rate gulf_mean gulf_stddev
	Calculates the average annual hurricane loss in $Billions for a simple hurricane model.The model is parameterized by :

	florida_landfall_rate - The annual rate of landfalling hurricanes in Florida
	florida_mean, florida_stddev - The LogNormal parameters that describe the economic loss of a landfalling hurricane in Florida.
	gulf_landfall_rate - The annual rate of landfalling hurricanes in the Gulf states
	gulf_mean, gulf_stddev - The LogNormal parameters that describe the economic loss of a landfalling hurricane in the Gulf states.
	options : -n, --num_monte_carlo_samples Number of samples (i.e.simulation years) to run

/////////////////////////////////////////////////////////////////////////////
