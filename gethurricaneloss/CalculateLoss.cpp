#include "CalculateLoss.h"
#include <string>
#include <cstring>
#include <sstream>
#include <memory>

namespace {

	bool get_param(double& param, const std::string& s_param, const std::string& name, std::string& msg)
	{
		try
		{
			param = std::stod(s_param, nullptr);
		}
		catch (const std::invalid_argument& ex)
		{
			std::ostringstream os;
			os << "Invalid argument " << s_param << " for " << name << ". " << std::endl;
			msg = os.str();
			return false;
		}
		catch (const std::out_of_range& ex)
		{
			std::ostringstream os;
			os << "Out of range value " << s_param << " for " << name << ". " << std::endl;
			msg = os.str();
			return false;
		}
		catch (...)
		{
			std::ostringstream os;
			os << "Failed to convert " << s_param << " for " << name << std::endl;
			msg = os.str();
			return false;
		}
		if (param <= 0 || param >= std::numeric_limits<double>::max()) 
		{
			std::ostringstream os;
			os << name << " souhld be positive and lower than " << std::numeric_limits<double>::max() << std::endl;
			msg = os.str();
			return false;
		}
		return true;
	}
}

CalculateLoss::CalculateLoss() :
	m_number_of_years(1000000),
    m_florida_landfall_rate(1.0),
    m_florida_mean(1.0),
    m_florida_stddev(1.0),
    m_gulf_landfall_rate(1.0),
    m_gulf_mean(1.0),
    m_gulf_stddev(1.0)
{}

double CalculateLoss::calculate_loss()
{
	if (m_hurricane_zones.empty())
	{
		return -1;
	}

	std::vector<std::thread> threads;
	unsigned number_of_threads = std::thread::hardware_concurrency() - 2;
	if (number_of_threads < 2)
	{
		double losses = 0.0;
		for (auto& zone : m_hurricane_zones)
		{
			double loss = 0;
			zone.calculate(m_number_of_years, loss);
			losses += loss;
		}
		return losses / m_number_of_years;
	}

	std::vector<double> losses(number_of_threads, 0);
	unsigned number_of_threads_per_zone = number_of_threads / m_hurricane_zones.size();
	unsigned number_of_years_per_thread = m_number_of_years / number_of_threads_per_zone;
	unsigned number_of_years_per_last_thread = number_of_years_per_thread + m_number_of_years % number_of_threads_per_zone;
	print_threads_info(std::cout, number_of_threads_per_zone, number_of_years_per_thread, number_of_years_per_last_thread);
	
	unsigned j = 0;
	for (auto& zone : m_hurricane_zones)
	{
		for (unsigned ii = 0; ii < number_of_threads_per_zone - 1; ++ii)
		{
			threads.push_back(std::thread(&HurricaneZone::calculate, zone, number_of_years_per_thread, std::ref(losses[j++])));
		}
		threads.push_back(std::thread(&HurricaneZone::calculate, zone, number_of_years_per_last_thread, std::ref(losses[j++])));
	}

	for (auto& t : threads)
	{
		t.join();
	}

	double loss = 0.0;
	for (auto& x : losses)
	{
		loss += x;
	}
	return loss / m_number_of_years;
}

void CalculateLoss::print_usage(std::ostream& os) const
{
	os << "usage: gethurricaneloss [options] florida_landfall_rate florida_mean florida_stddev gulf_landfall_rate gulf_mean gulf_stddev" << std::endl;
	os << "Calculates the average annual hurricane loss in $Billions for a simple hurricane model.The model is parameterized by :" << std::endl;
	os << "florida_landfall_rate - The annual rate of landfalling hurricanes in Florida" << std::endl;
	os << "florida_mean, florida_stddev - The LogNormal parameters that describe the economic loss of a landfalling hurricane in Florida." << std::endl;
	os << "gulf_landfall_rate - The annual rate of landfalling hurricanes in the Gulf states" << std::endl;
	os << "gulf_mean, gulf_stddev - The LogNormal parameters that describe the economic loss of a landfalling hurricane in the Gulf states." << std::endl;
	os << "options : -n, --num_monte_carlo_samples Number of samples (i.e.simulation years) to run" << std::endl;
}

void CalculateLoss::print_params(std::ostream& os) const
{
	os << "Calculating average annual hurricane loss in $Billions for the following params: " << std::endl;
	os << "number of simulation years: " << m_number_of_years << std::endl;
	os << "florida_landfall_rate: " << m_florida_landfall_rate  << std::endl;
	os << "florida_mean: " << m_florida_mean << std::endl;
    os << "florida_stddev: " << m_florida_stddev << std::endl;
	os << "gulf_landfall_rate: " << m_gulf_landfall_rate << std::endl;
	os << "gulf_mean: " << m_gulf_mean << std::endl;
	os << "gulf_stddev: " <<  m_gulf_stddev << std::endl;
}

bool CalculateLoss::validate_params(int argc, char *argv[], std::string& msg)
{
	if (argc == 1)
	{
		return false;
	}

	if (argc < 7 || argc > 9 || argc == 8)
	{
		msg = "Wrong number of parameters";
		return false;
	}

	size_t ii = 1;
	m_number_of_years = 1000000;
	if (!strcmp(argv[ii], "-n") || !strcmp(argv[ii], "--num_monte_carlo_samples"))
	{
		if (argc != 9)
		{
			msg = "Wrong number of parameters";
			return false;
		}

		std::string s_number_of_years(argv[++ii]);
		if (s_number_of_years[0] == '-')
		{
			msg = "The number of years should be positive";
			return false;
		}

		try
		{
			m_number_of_years = std::stoul(s_number_of_years, nullptr, 0);
		}
		catch (const std::invalid_argument& ex)
		{
			std::ostringstream os;
			os << "Invalid argument for number of samples." <<  std::endl;
			msg = os.str();
			return false;
		}
		catch (const std::out_of_range& ex)
		{
			std::ostringstream os;
			os <<"Out of range value for number of samples. " << ex.what() << std::endl;
			msg = os.str();
			return false;
		}
		catch (...)
		{
			std::ostringstream os;
			os << "Failed to get number of samples." << std::endl;
			msg = os.str();
			return false;
		}
		
		if (m_number_of_years < 1 || m_number_of_years >= std::numeric_limits<unsigned>::max()) 
		{
			std::ostringstream os;
			os << "The number of samples should be between 1 and " << std::numeric_limits<unsigned>::max() << std::endl;
			msg = os.str();
			return false;
		}
		++ii;
	}
	
	//The annual rate of landfalling hurricanes in Florida
	std::string s_florida_landfall_rate(argv[ii++]);
	if (!get_param(m_florida_landfall_rate, s_florida_landfall_rate, "Florida landfall rate", msg))
	{
		return false;
	}
	
	//florida_mean; 
	std::string s_florida_mean(argv[ii++]);
	if (!get_param(m_florida_mean, s_florida_mean, "Florida mean", msg))
	{
		return false;
	}
	
	//florida_stddev
	std::string s_florida_stddev(argv[ii++]);
	if (!get_param(m_florida_stddev, s_florida_stddev, "Florida stddev", msg))
	{
		return false;
	}
	
	//The annual rate of landfalling hurricanes in the Gulf states
	std::string s_gulf_landfall_rate(argv[ii++]);
	if (!get_param(m_gulf_landfall_rate, s_gulf_landfall_rate, "Gulf landfall rate", msg))
	{
		return false;
	}
	
	//gulf_mean;
	std::string s_gulf_mean(argv[ii++]);
	if (!get_param(m_gulf_mean, s_gulf_mean, "Gulf mean", msg))
	{
		return false;
	}
	
	//gulf_stddev; 
	std::string s_gulf_stddev(argv[ii++]);
	if (!get_param(m_gulf_stddev, s_gulf_stddev, "Gulf stddev", msg))
	{
		return false;
	}

	return true;
}


double CalculateLoss::get_loss()
{
	LandfallAnnualEvents::LandfallAnnualEventsPtr land_fall_events_florida(std::make_shared<LandfallAnnualEvents>(m_florida_landfall_rate));
	EconomicLoss::EconomicLossPtr economic_loss_florida(std::make_shared<EconomicLoss>(m_florida_mean, m_florida_stddev));
	LandfallAnnualEvents::LandfallAnnualEventsPtr land_fall_events_gulf(std::make_shared<LandfallAnnualEvents>(m_gulf_landfall_rate));
	EconomicLoss::EconomicLossPtr economic_loss_gulf(std::make_shared<EconomicLoss>(m_gulf_mean, m_gulf_stddev));
	
	m_hurricane_zones.push_back(HurricaneZone(land_fall_events_florida, economic_loss_florida));
	m_hurricane_zones.push_back(HurricaneZone(land_fall_events_gulf, economic_loss_gulf));
	return calculate_loss();
}
	
void CalculateLoss::print_threads_info(std::ostream& os, unsigned number_of_threads_per_zone, unsigned number_of_years_per_thread, unsigned number_of_years_per_last_thread)
{
	std::cout << "number of years=" << m_number_of_years << "; max number of threads per zone=" << number_of_threads_per_zone << "." << std::endl;
	if (number_of_years_per_last_thread == number_of_years_per_thread)
	{
		std::cout << number_of_threads_per_zone << " threads running " << number_of_years_per_thread << " years per thread" << std::endl;
	}
	else
	{
		if (number_of_years_per_thread)
		{
			std::cout << number_of_threads_per_zone - 1 << " threads running " << number_of_years_per_thread << " years per thread per zone";
		}
		if (number_of_years_per_last_thread)
		{
			std::cout << "one thread running " << number_of_years_per_last_thread << " years per zone.";
		}
		std::cout << std::endl;
	}
}