#!/usr/bin/python3

import getopt, sys
import re
import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


time_token = 'Average:'

file_crawler = {
    'program_name': './a.out',
}

sieve1 = {
    'program_name': './sieve1',
}

# [program] Dict
def collect_data(program):
    times = []
    thread_means = []
    for threads in range(1, 9):
        print("Threads:", threads)
        os.environ['OMP_NUM_THREADS'] = str(threads)
        for size in [500000000, 1000000000, 1500000000]:
            results = []
            for i in range(0, 8): 
                output = subprocess.run([program['program_name']] + str(size).split(), capture_output=True).stdout.decode('utf-8')
                result = re.search('(?<=' + program['time_token'] + ').\S*', output).group(0).strip()
                print(threads, size, result)
                results.append(float(result))
            results.remove(max(results))
            results.remove(min(results))
            mean = sum(results)/len(results)
            thread_means.append(threads, size, mean)
    return thread_means 

# [program] Dict
def collect_seq_data(program):
    results = []
    for i in range(0, 8): 
        output = subprocess.run([program['program_name']] + str('true 1').split(), capture_output=True).stdout.decode('utf-8')
        result = re.search('(?<=' + time_token + ').\S*', output).group(0).strip()
        print(result)
        results.append(float(result))
    results.remove(max(results))
    results.remove(min(results))
    mean = sum(results)/len(results)
    return mean 


def calculate_speedup(baseline_mean, test_times):
    speedup_list = []
    for time in test_times:
        speedup = baseline_mean / time * 100
        speedup_list.append(speedup)
    return speedup_list


#sizes =  [10000, 20000, 30000]
#sizes =  [100000, 200000, 300000]

# Uncomment this to compare sieve and sieve1
baseline_results = collect_seq_data(file_crawler)
print("Mean for 8 trials:", baseline_results)


#sieve1_results = collect_seq_data(sieve1, sizes)
"""
improvements = []
for sieve, sieve1 in zip(baseline_results, sieve1_results):
    result = sieve - sieve1 / 10
    improvements.append(result)


seq_data = pd.DataFrame({
    "Problem Size": sizes,
    "Magnitude Improvement (10x)": improvements,
})


filename = 'sieve'
seq_data.to_csv(filename + '.csv', index = False)
fig = plt.figure()
ax = fig.add_subplot(111)

ax.plot(seq_data['Problem Size'], seq_data['Magnitude Improvement (10x)'], label = 'Magnitude Improvement (10x)')

plt.xlabel('Problem Size')
plt.ylabel('Magnitude Improvement (s)')

#plot_multi(data, figsize=(10, 5))
#plt.xticks(np.arange(8), np.arange(1, 9))
#plt.subplots_adjust(right=0.8)
plt.title('Sieve1 vs Sieve' + ' Statistics')
#plt.legend()
plt.savefig('sieve1' + '.png')
plt.show()    
"""
