#pragma once

#include <mutex>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/normal_distribution.hpp>

using namespace boost::random;

class NormalDistributionParams
{
protected:
	NormalDistributionParams(double mean, double stddev) :
		m_gamma(1 + stddev * stddev / (mean * mean)),
		m_mu(log(mean / sqrt(m_gamma))),
		m_sigma(sqrt(log(m_gamma)))
	{}
	double m_gamma;
	double m_mu;
	double m_sigma;
};

class EconomicLoss : private NormalDistributionParams
{
public:
	EconomicLoss(double mean, double stddev) :
		NormalDistributionParams(mean, stddev),
		m_mean(mean),
		m_stddev(stddev),
		m_distribution(m_mu, m_sigma)
	{}

	double get_economic_loss() const
	{
		std::lock_guard<std::mutex> lck(m_mtx);
		return exp(m_distribution(m_random_generator));
	}

	typedef std::shared_ptr<EconomicLoss> EconomicLossPtr;

private:
	const double m_mean;
	const double m_stddev;
	mutable normal_distribution<double> m_distribution;
	mutable std::mutex m_mtx;
	mutable minstd_rand0 m_random_generator;
};





