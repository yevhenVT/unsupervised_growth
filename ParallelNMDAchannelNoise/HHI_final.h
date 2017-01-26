#pragma once
#ifndef HHI_FINAL_H
#define HHI_FINAL_H

#include <vector>
#include <functional>
#include <cmath>

class HH2_final;
class Poisson_noise;

using std::vector;
typedef std::function<double (double)> DDfunction;

class HHI_final
{
public:
	HHI_final();
	
	// print
	void print_param(); // print parameters of the model
	void print_targets(); // print targets of a neuron
	
	// get internal states of dynamics
	int get_spike_number();	//	get number of action potential spikes
	bool get_fired(); // get if neuron fired AP
	vector <int> get_activity();	//	get activity of a neuron during dynamics

	// get target information
	vector<int> get_targetsID(); // get IDs of target neurons
	vector<double> get_targetsG(); // get conductances of target neurons
	
	// calculate dynamics
	void R4_step_with_target_update(); // do one step of RK4 with update of target conductances
	void R4_step_no_target_update(); // do one step of RK4 without update of target conductances
	void Runge4_step();	//	do one step of Runge-Kutta order 4
	
	void DP5_step_no_target_update(); // do one step of Dormand Prince order 5 without update of target conductances
	void DP5_step(); // do one step of Dormand Prince order 5 method 
	
	void PC4_step_no_target_update(); // do one predictor-corrector step of order 4 without update of target conductances
	void AB4_step_no_target_update();	//	do one step of Adams-Bashforth order 4 method without target update
	void AB4_step();	//	do one step of Adams-Bashforth order 4 method
	void AM4_step();	//	do one step of Adams-Moulton order 4 method preceded by Adams-Bashforth order 4 method
	
	void R6_step_no_target_update(); // do one step of RK6 without update of target conductances
	void Runge6_step();	//	do one step of Runge-Kutta order 6
	
	void DP8_step_no_target_update(); // do one step of Dormand Prince order 8 without update of target conductances
	void DP8_step(); // do one step of Dormand Prince order 8 method 
	
	void Euler_step_no_target_update(); // do one Euler step without update of target conductances
	void Euler_step(); // do one Euler step

	void iEuler_step_no_target_update(); // do one implicit Euler step without update of target conductances
	void iEuler_step(); // do one step of implicit Euler

	void iTrapezoid_step_no_target_update(); // do one implicit Trapezoid step without update of target conductances
	void iTrapezoid_step(); // do one step of implicit Trapezoid
	
	void BDF4_step_no_target_update(); // do one BDF4 step without update of target conductances
	void BDF4_step(); // do one BDF4 step
	
	void BDF6_step_no_target_update(); // do one BDF6 step without update of target conductances
	void BDF6_step(); // do one BDF6 step

	// noise
	void set_no_poisson_noise(); //	turn off Poisson noise
	void set_no_white_noise(); // turn off white noise
    void set_noise_generator(Poisson_noise* g);	//	set pointer to Poisson noise generator 
	void set_white_noise(double m, double s); // set white-noise injected current
    void set_poisson_noise(); // enable Poisson noise

	// miscalleneous
	void set_target(HH2_final* target, int ID, double G);	//	function to set a target for neuron
	void set_dynamics(double interval, double tS);	//	function to set the dynamic range of neuron (time step and time interval)
	
	void raiseE(double G);	//	increase excitatory conductance of the neuron due to excitatory spike
	void raiseI(double G);	//	increase inhibitory conductance of the neuron due to inhibitory spike

	void set_injected_current(DDfunction f); // set a function for injected current
	
	void set_to_rest(); // set all variables to the resting state

	void writeToFile(const char * filename);	//	function to write data to binary file
	
	
protected:
	// constants and parameters of the cell
	
	double cm;	//	membrane capacitance
	double A;	//	neuron's area

	double Ena;	//	equilibrium potential of sodium ions
	double Ek;	//	equilibrium potential of potassium ions
	double El;	//	equilibrim membrane potential
	double Ei;	//	inhibitory reversal potential

	double gNa;	//	maximum sodium channels conductance (all open)
	double gKdr;	//	delay-rectified potassium channel conductance 
	double gKHT;	//	high-treshold potassium channel conductance
	double gL;	//	membrane leakage conductance
	
	double tExc;	//	time scale of excitatory conductance decay
	double tInh;	//	time scale of inhibitory conductance decay

	double threshold;	//	threshold indicator for action potential generator

	// dynamics variables
	
	vector<double> voltage;
	vector<double> time;
	
	vector<double> n;		// vector that contains values of gating variable of delay-rectified potassium (K+) channel
	vector<double> m;		// vector that contains values of activation gating variable of sodium (Na+) channel
	vector<double> h;		// vector that contains values of inactivation gating variable of sodium (Na+) channel
	vector<double> w;		// vector that contains values of gating variable of high-threshold potassium (K+) channel 
	vector<double> I;		// vector that contains injected current values
	vector<double> Gexc;	//	vector that contains total conductance of excitatory synapses
	vector<double> Ginh;	//	vector that contains total conductance of inhibitory synapses
	
	// targets of a neuron
	
	vector <HH2_final*> targets;	//	vector which contains pointers to target neurons
	vector <int> targetsID;	//	vector which contains targets' ID in layer
	vector <double> targetsG;	//	vector which contains synaptic conductance to target neurons

	// supportive internal state parameters of dynamics
	
	int itime;	//	internal time of neuron
	vector<int> flag;	//	vector-indicator of crossing threshold
	int Nspikes;	//	number of spikes occured during dynamics
	bool fired = false; // state of neuron (fired/silent)
	double timeStep;	//	time step for solving the dynamics of the model
	int size; // size of dynamics arrays
	
	// internal state check functions
	
	void noise_check(double& G, int& noise_time); // check noise
	void state_check(); // check if neuron fired
	void state_noise_check(); // check both states
	
	//	Functions to change conductance of target neurons

	void postsyn_update();	//	change all postsynaptic conductances	

	//	conductance
	double Gi(double t);	//	calculate inhibitory conductance at time point t
	double Ge(double t);	//	calculate excitatory conductance at time point t
	
	//	Noise
	
	Poisson_noise* generator;	//	pointer to Poisson noise generator

    // poisson noise
	bool poisson_noise;	//	indicator for Poisson noise
	int noise_inh;	//	time of inhibitory noisy spike rounded to the time grid
	int noise_exc;	//	time of excitatory noisy spike rounded to the time grid
	double G_noise;	//	maximum noise conductance added to either inhibitory							//	or excitatory conductance of neuron
	double lambda;	//	parameter of Poisson point process
	
	void initialize_noise(int& noise_time); // initialize noise spike times

    // white noise
    double mu; // white noise mean
    double sigma; // white noise standard deviation

	//	current
	bool injected_current; // indicator that external current is injected 
	double I_total(double t); // total current 
	DDfunction I_injected; // injected current

	double I_default(double t){return 0;}; // default function for injected current (returns zero)	
	
	
	double bisection(DDfunction f, double x, double xmax, double eps, int Nmax); // bisection method for finding solution of nonlinear equation
	
	// support for predictor-corrector model
	vector<double> fv; // right handside of ode for voltage
	vector<double> fn; // right handside of ode for n
	vector<double> fm; // right handside of ode for m
	vector<double> fh; // right handside of ode for h
	vector<double> fw; // right handside of ode for w


	// support for BDF6
	double m_bdf6(double v);
	double n_bdf6(double v);
	double h_bdf6(double v);
	double w_bdf6(double v);
	double f_bdf6(double v);
	
	// support for BDF4
	double m_bdf4(double v);
	double n_bdf4(double v);
	double h_bdf4(double v);
	double w_bdf4(double v);
	double f_bdf4(double v);
	
	// support for implicit Trapezoid
	double m_iTrapezoid(double fm, double v);
	double n_iTrapezoid(double fn, double v);
	double h_iTrapezoid(double fh, double v);
	double w_iTrapezoid(double fw, double v);
	double f_iTrapezoid(double fv, double fn, double fm, double fh, double fw, double v);

	// support for implicit Euler
	double m_iEuler(double v);
	double n_iEuler(double v);
	double h_iEuler(double v);
	double w_iEuler(double v);
	double f_iEuler(double v);

	// support functions for Runge-Kutta method
	double kV(double v, double t, double h, double w, double m3, double n4);
	double kn(double v, double n);
	double km(double v, double m);
	double kh(double v, double h);
	double kw(double v, double w);

	static double an(double v){return 0.15*(v + 15) / (1 - exp(-(v + 15) / 10));} // was 0.05; original value = 0.15
	static double bn(double v){return 0.2 * exp(-(v + 25) / 80);} // was 0.1; original value = 0.2
	static double am(double v){return (v + 22) / (1 - exp(-(v + 22) / 10));}
	static double bm(double v){return 40 * exp(-(v + 47) / 18);}
	static double ah(double v){return 0.7 * exp(-(v + 34) / 20);}
	static double bh(double v){return 10 / (1 + exp(-(v + 4) / 10));}
	static double wInf(double v){return 1 / (1 + exp(-v / 5));}
	static double tauW(double v){return 1;} // was 2; original value = 1
};



#endif
