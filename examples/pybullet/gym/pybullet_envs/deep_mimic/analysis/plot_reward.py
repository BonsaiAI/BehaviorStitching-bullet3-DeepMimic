import numpy as np
import matplotlib.pyplot as plt

log_filename = '../spinkick_output/agent0_log.txt'

A = np.genfromtxt(log_filename,delimiter=None,dtype=None, names=True)

x = A['Iteration']
y = A['Train_Return']
s = A['Samples']

fig, ax = plt.subplots()
ax.plot(s, y)

ax.set(xlabel='Samples', ylabel='Train Return',
       title='Train Return vs. Iterations')
# ax.grid()

fig.savefig("test1.png")
plt.show()
