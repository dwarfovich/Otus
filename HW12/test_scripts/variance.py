import numpy as np

for i in range(2, 6):
    array = list(range(1, i + 1));
    print(array, ":");
    print(i, np.var(array));
    print('\n');
