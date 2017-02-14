# -*- coding: utf-8 -*-
"""
Created on Mon Feb 13 18:23:26 2017

@author: jingroup

Script checks supersynapses and active connections
"""

import reading
import numpy as np

filename_before_active = "/home/eugene/Output/networks/gabaMaturation100217/RA_RA_active_connections_before_neuron_addition.bin"
filename_after_active = "/home/eugene/Output/networks/gabaMaturation100217/RA_RA_active_connections_after_neuron_addition.bin"

(N_RA_before, targets_ID_before_active, targets_G_before_active) = reading.read_connections(filename_before_active)
(N_RA_after, targets_ID_after_active, targets_G_after_active) = reading.read_connections(filename_after_active)

print targets_ID_before_active
print targets_ID_after_active



