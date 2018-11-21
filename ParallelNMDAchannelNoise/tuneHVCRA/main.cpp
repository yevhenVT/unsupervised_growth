#include <iostream>
#include "../HH2_test.h"
#include "../poisson_noise.h"
#include <functional>
#include <fstream>
#include <vector>
#include <sys/stat.h>

using namespace std::placeholders;

// target neuron model parameters
double E_GABA_MATURE = -80.000000;
double E_GABA_IMMATURE = -80.000000; // was -55.0

double E_REST_MATURE = -80.000000;
double E_REST_IMMATURE = -55.000000; // was -55.0

double AD_MATURE = 10000.000000;
double AD_IMMATURE = 10000.000000; // was 1000

double GK_MATURE = 8.000000;
double GK_IMMATURE = 8.000000; // was 16.0

double GNA_MATURE = 60.000000;
double GNA_IMMATURE = 60.000000; // was 40.0

double RC_MATURE = 55.000000; // was 55.0
double RC_IMMATURE = 55.000; // was 5.5

double GCA_MATURE = 55.0; // original 55.0
double GCA_IMMATURE = 0.0;

double GCAK_MATURE = 150.0;
double GCAK_IMMATURE = 150.0;

double GSL_MATURE = 0.1; // original 0.1
double GSL_IMMATURE = 0.1; // original 0.1

double GDL_MATURE = 0.1; // original 0.1
double GDL_IMMATURE = 0.1; // original 0.1

double age = 0.0;

double gaba_potential = E_GABA_MATURE + (E_GABA_IMMATURE - E_GABA_MATURE) * exp(-age);
double rest_potential = E_REST_MATURE + (E_REST_IMMATURE - E_REST_MATURE) * exp(-age);
double Gk = GK_MATURE + (GK_IMMATURE - GK_MATURE) * exp(-age);
double GNa = GNA_MATURE + (GNA_IMMATURE - GNA_MATURE) * exp(-age);
double Ad = AD_MATURE + (AD_IMMATURE - AD_MATURE) * exp(-age);
double Rc = RC_MATURE + (RC_IMMATURE - RC_MATURE) * exp(-age);
double GCa = GCA_MATURE + (GCA_IMMATURE - GCA_MATURE) * exp(-age);
double GCaK = GCAK_MATURE + (GCAK_IMMATURE - GCAK_MATURE) * exp(-age);
double GsL = GSL_MATURE + (GSL_IMMATURE - GSL_MATURE) * exp(-age);
double GdL = GDL_MATURE + (GDL_IMMATURE - GDL_MATURE) * exp(-age);
	
double I(double ampl, double start, double t)
{
	double duration = 20;
	
	if ( ( t >= start ) && ( t <= (start + duration) ) )
		return ampl;
	else
		return 0;
}

void write_fI(int N, double ampl_step, std::vector<int>& num_spikes, 
						std::vector<int>& num_bursts, const char *filename)
{
	std::ofstream out;
	out.open(filename, std::ios::binary | std::ios::out);
	
	out.write(reinterpret_cast<char*>(&N), sizeof(int));
	out.write(reinterpret_cast<char*>(&ampl_step), sizeof(double));
	
	for (int i = 0; i < N; i++)
	{
		out.write(reinterpret_cast<char*>(&num_spikes[i]), sizeof(int));
		out.write(reinterpret_cast<char*>(&num_bursts[i]), sizeof(int));
	}
	out.close();
}

void write_conductance_response(int N, double G_step,
						std::vector<double>& burst_onset_times, std::vector<double>& spike_times,
						const char *filename)
{
	std::ofstream out;
	out.open(filename, std::ios::binary | std::ios::out);
	
	out.write(reinterpret_cast<char*>(&N), sizeof(int));
	out.write(reinterpret_cast<char*>(&G_step), sizeof(double));
	
	for (int i = 0; i < N; i++)
	{
		out.write(reinterpret_cast<char*>(&burst_onset_times[i]), sizeof(double));
		out.write(reinterpret_cast<char*>(&spike_times[i]), sizeof(double));
	}
	
	out.close();
}

void calculate_fI(int N, double ampl_step, bool injection_to_soma, std::string dirname)
{
	double timestep = 0.02;
	double sim_duration = 100;
	double start = 50.0;
	
	int num_iter = static_cast<int>(sim_duration / timestep);
	
	std::vector<int> num_spikes(N);
	std::vector<int> num_bursts(N);
	
	
	double ampl = 0.0;
	unsigned seed = 1991;
	
	Poisson_noise noise_generator;
	
	noise_generator.set_seed(seed);
	
	std::string filename;
	
	if (injection_to_soma)
		filename = dirname + "fI_soma.bin";
	else
		filename = dirname + "fI_dend.bin";
	
	for (int i = 0; i < N; i++)
	{
		HH2_test *neuron = new HH2_test;
	
	
		std::function<double (double)> f = std::bind(&I, ampl, start, _1);
	
		neuron->set_dynamics(timestep);
		
		neuron->set_Ei(gaba_potential);
		neuron->set_Erest(rest_potential);
		
		neuron->set_Gk(Gk);
		neuron->set_GNa(GNa);
		neuron->set_GCa(GCa);
		neuron->set_GCaK(GCaK);
		neuron->set_GsL(GsL);
		neuron->set_GdL(GdL);
		
		neuron->set_Ad(Ad);
		neuron->set_Rc(Rc);
		
		
		
		std::string filename_neuron;
		
		if (injection_to_soma)
			filename_neuron = dirname + "RA_soma_" + std::to_string(i) + ".bin";
		else
			filename_neuron = dirname + "RA_dend_" + std::to_string(i) + ".bin";
		
		struct stat buf;
	
		if ( stat(filename_neuron.c_str(), &buf) == 0 )
			std::remove(filename_neuron.c_str());
		
		
		neuron->set_recording_full(filename_neuron);
		
		if (injection_to_soma)
			neuron->set_soma_current(f);
		else
			neuron->set_dend_current(f);
			
		neuron->set_noise_generator(&noise_generator);
		neuron->set_white_noise(0.0, 0.0, 0.0, 0.0);
	
		for (int j = 0; j < num_iter; j++)
			neuron->Debraband_step_no_target_update();	

		num_spikes[i] = neuron->get_spike_number_soma();
		num_bursts[i] = neuron->get_spike_number_dend();
		
		delete neuron;
		ampl += ampl_step;
	}
	
	write_fI(N, ampl_step, num_spikes, num_bursts, filename.c_str());
}


void calculate_conductance_response(int N, double G_step, std::string dirname)
{
	double timestep = 0.02;
	double sim_duration = 300;
	double kick_time = 50.0;
	bool excited;
	bool spiked;
	
	int num_iter = static_cast<int>(sim_duration / timestep);
	
	std::vector<double> burst_onset_times(N);
	std::vector<double> spike_times(N);
	
	std::fill(burst_onset_times.begin(), burst_onset_times.end(), -1.0);
	std::fill(spike_times.begin(), spike_times.end(), -1.0);
	
	double G = 0.0;
	unsigned seed = 1991;
	
	Poisson_noise noise_generator;
	
	noise_generator.set_seed(seed);
	
	for (int i = 0; i < N; i++)
	{
		HH2_test *neuron = new HH2_test;
	
		neuron->set_dynamics(timestep);
		
		neuron->set_Ei(gaba_potential);
		neuron->set_Erest(rest_potential);
		
		neuron->set_Ad(Ad);
		neuron->set_Rc(Rc);
		
		neuron->set_Gk(Gk);
		neuron->set_GNa(GNa);
		neuron->set_GCa(GCa);
		neuron->set_GCaK(GCaK);
		neuron->set_GsL(GsL);
		neuron->set_GdL(GdL);
		
		
		neuron->set_noise_generator(&noise_generator);
		//neuron->set_white_noise(0.0, 0.1, 0.0, 0.2);
		neuron->set_white_noise(0.0, 0.0, 0.0, 0.0);
	
		excited = false;
		spiked = false;
	
		std::string filename = dirname + "RA" + std::to_string(i) + ".bin";
		
		struct stat buf;
	
		if ( stat(filename.c_str(), &buf) == 0 )
			std::remove(filename.c_str());
		
		
		neuron->set_recording_full(filename);
		
	
	
		for (int j = 0; j < num_iter; j++)
		{
			neuron->Debraband_step_no_target_update();	
			
			if ( ( static_cast<double>(j) * timestep > kick_time ) && ( !excited ) )
			{
				neuron->raiseE(G);
				excited = true;
			}
			
			if ( neuron->get_fired_dend() )
				burst_onset_times[i] = static_cast<double>(j) * timestep - kick_time;
				
			if ( ( neuron->get_fired_soma() ) && ( !spiked ) )
			{
				spike_times[i] = static_cast<double>(j) * timestep - kick_time;
				spiked = true;
			}
		}
		
		
		delete neuron;
		G += G_step;
	}
	
	std::string filename_G = dirname + "G_response.bin";
	
	write_conductance_response(N, G_step, burst_onset_times, spike_times, filename_G.c_str());
	
}

int main(int argc, char **argv)
{
	std::string dirname = "/mnt/hodgkin/eugene/Output/tuneHVCRA/finalModel/KLTH3.5Rest55mVLargeIrange/age0/";
	
	int N = 50;
	double ampl_step = 0.080; // was 0.025
	
	
	
	std::cout << "E_GABA = " << gaba_potential << "\n"
			  << "E_REST = " << rest_potential << "\n"
			  << "G_K = "    << Gk             << "\n"
			  << "G_NA = "   << GNa            << "\n"
			  << "G_CA = "   << GCa            << "\n"
			  << "G_CAK = "  << GCaK           << "\n"
			  << "G_SL = "   << GsL            << "\n"
			  << "G_DL = "   << GdL            << "\n"
			  << "Ad = "     << Ad             << "\n"
			  << "Rc = "     << Rc             << std::endl;
	
	calculate_fI(N, ampl_step, true, dirname);
	calculate_fI(N, ampl_step, false, dirname);
	
	double G_step = 0.10; // was 0.05
	N = 100;
	
	calculate_conductance_response(N, G_step, dirname);
}
