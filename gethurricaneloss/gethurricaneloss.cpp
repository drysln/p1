/*
gethurricaneloss
Calculates the average annual hurricane loss in $Billions for a simple hurricane model.
*/

#include "CalculateLoss.h"
#include <ostream>
#include <string>

int main(int argc, char *argv[])
{
	CalculateLoss caluclateLoss;

	std::string msg;
	if (!caluclateLoss.validate_params(argc, argv, msg))
	{
		std::cerr << msg << std::endl;
		caluclateLoss.print_usage(std::cout);
		return -1;
	}

	std::cout << std::endl;
	caluclateLoss.print_params(std::cout);
	std::cout << std::endl;

	double loss = caluclateLoss.get_loss();
	if (loss < 0.0)
	{
		std::cerr << "Failed to calculate the loss. Please check your params." << std::endl;
		return -1;
	}
	std::cout << "loss: " << loss << std::endl;
	return 0;
}
