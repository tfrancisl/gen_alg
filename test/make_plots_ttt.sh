#!/usr/bin/bash

for i in {0..1}
do
    python3 fitness_plot_ttt.py test$i\_$1.csv test$i\_$1
done