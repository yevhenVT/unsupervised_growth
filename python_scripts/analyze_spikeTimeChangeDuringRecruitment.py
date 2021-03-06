# -*- coding: utf-8 -*-
"""
Created on Wed Mar 28 11:50:08 2018

@author: jingroup

Script plots spike times of neurons relative to spike times of training
"""
import utils
import reading
import matplotlib.pyplot as plt

#dirname = "/home/eugene/Output/networks/chainGrowth/passiveDendrite/noImmatureOut5/"
dirname = "/home/eugene/results/immature/clusters/matTrans15/"

trial_number = 3000
trialStep = 50

training_neurons = [669, 822, 291, 228, 726, 807, 909, 493, 285, 592, 850, 179, 886, 438, 23, 824, 196, 964, 221, 127]
neurons = [770, 774, 264, 777, 139, 404, 593, 280, 29, 287, 163]
N_RA = 1000

spike_times_sequence = utils.get_first_spike_time_sequence_relative_to_training(dirname, trial_number, trialStep, N_RA, neurons, training_neurons)
maturation_indicators = utils.get_maturation_time_sequence(dirname, trial_number, trialStep, neurons)

time = [i*trialStep for i in range(trial_number / trialStep +1)]

num_neurons = len(neurons)

f, axarr = plt.subplots(num_neurons, sharex=True)
#axarr[i].set_ylim([-0.1,1.1])

for i in range(num_neurons):
    axarr[i].plot(time, spike_times_sequence[i], label="neuron {0}".format(neurons[i]), color='b')
    axarr[i].set_ylim([5, 15])
    axarr[i].tick_params('y', color='b')
    axarr[i].set_ylabel('FST (ms)', color='b')
    axarr[i].legend(loc=2)
    
    ax = axarr[i].twinx()
    ax.plot(time, maturation_indicators[i], color='r')    
    ax.tick_params('y', color='r')
    ax.set_ylabel('mature', color='r')
    ax.set_ylim([-0.1, 1.1])
    
#axarr[num_neurons].plot(time, maturation_indicators[0])    
#axarr[num_neurons].set_xlabel('time in # of trials')
#axarr[num_neurons].set_ylabel('mature')
#axarr[num_neurons].set_ylim([-0.1, 1.1])
    
    
plt.show()