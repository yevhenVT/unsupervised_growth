#!/bin/sh
# run program with following command line arguments: 
# mu_soma, sigma_soma, mu_dend, sigma_dend,
# Gie_mean, Ei, BETA, BETA_SUPERSYNAPSE, T_P, T_D, TAU_P, TAU_D,  A_P, A_D, A_P_SUPER, A_D_SUPER, f0, ACTIVATION, 
# SUPERSYNAPSE_THRESHOLD, G_MAX, N_RA, num_inh_clusters_in_row, num_inh_in_cluster, N_ss, N_TR, outputFolder,
# reading, filenumber, testing, training,  network_update_frequency, A_RA2I, SIGMA_RA2I, B_I2RA, SIGMA_I2RA 
mpirun -np 23 ./out 0 200 0 330 0.5 -55 0.9998 0.995 10 10 30 30 0.0007 0.00004 0.0015 0.00010 1.2 0.002 0.007 0.040 300 10 1 4 4 /home/eugene/Output/ 0 1 0 1 1.0 5.0 5.0 5.0 5.0
