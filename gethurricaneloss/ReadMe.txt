========================================================================
    gethurricaneloss Project Overview
========================================================================

This file contains a summary of what you will find in each of the files that
make up your gethurricaneloss application.

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

	Makefile uses two variables:
	BOOST_LIB - location of boost lib include files
	TARGET - target directory for make install
	
	make - builds gethurricaneloss.exe
	make install - builds and install in ${TARGET}
	make clean
	
/////////////////////////////////////////////////////////////////////////////
Other notes:
	run gethurricaneloss to see the required params
	

/////////////////////////////////////////////////////////////////////////////
