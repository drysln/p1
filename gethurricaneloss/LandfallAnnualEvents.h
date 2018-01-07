#pragma once

#include <boost/random/poisson_distribution.hpp>
#include <boost/random/linear_congruential.hpp>
#include <mutex>

using namespace boost::random;

class LandfallAnnualEvents
{
public:
	LandfallAnnualEvents(double landfall_rate) :
		m_landfall_rate(landfall_rate),
		m_distribution(m_landfall_rate)
	{}

	int get_number_of_annual_events() const
	{
		std::lock_guard<std::mutex> lck(m_mtx);
		return m_distribution(m_random_generator);
	}

	typedef std::shared_ptr<LandfallAnnualEvents> LandfallAnnualEventsPtr;

private:
	const double m_landfall_rate;
	poisson_distribution<int, double> m_distribution;
	mutable std::mutex m_mtx;
	mutable minstd_rand0 m_random_generator;
};



