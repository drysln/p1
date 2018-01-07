#pragma once

#include "HurricaneZone.h"
#include <vector>
#include <string>

class CalculateLoss
{
public:
	CalculateLoss();
	
	// Blocking call for getting the loss
	double get_loss();

	bool validate_params(int argc, char* argv[], std::string& msg);
	void print_usage(std::ostream& os) const;
	void print_params(std::ostream& os) const;

private:
	double calculate_loss();
	void print_threads_info(std::ostream& os, unsigned number_of_threads_per_zone, unsigned number_of_years_per_thread, unsigned number_of_years_per_last_thread);
	
	size_t m_number_of_years;
	double m_florida_landfall_rate;
	double m_florida_mean;
	double m_florida_stddev;
	double m_gulf_landfall_rate;
	double m_gulf_mean;
	double m_gulf_stddev;

	std::vector<HurricaneZone> m_hurricane_zones;
};


