#pragma once

#include "LandfallAnnualEvents.h"
#include "EconomicLoss.h"

class HurricaneZone
{
public:
	HurricaneZone(const LandfallAnnualEvents::LandfallAnnualEventsPtr& landfallAnnualEventsPtr, const EconomicLoss::EconomicLossPtr& economicLossPtr) :
		m_landfall_annual_events(landfallAnnualEventsPtr),
		m_economic_loss(economicLossPtr)
	{}

	void calculate(unsigned number_of_years, double& total_loss)
	{
		total_loss = 0.0;
		//For each simulation year
		for (unsigned ii = 0; ii < number_of_years; ++ii)
		{
			double simulation_loss = 0.0;
			int number_of_events = m_landfall_annual_events->get_number_of_annual_events();
			//For each  event
			for (int jj = 0; jj < number_of_events; ++jj)
			{
				//Sample a loss and add it to the simulation loss
				simulation_loss += m_economic_loss->get_economic_loss();
			}
			total_loss += simulation_loss;
		}
	}

private:
	LandfallAnnualEvents::LandfallAnnualEventsPtr m_landfall_annual_events;
	EconomicLoss::EconomicLossPtr m_economic_loss;
};


