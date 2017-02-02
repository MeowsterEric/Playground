# loading data from file
import matplotlib.pyplot as plt

FILE_NAME = "07_sample_csv.csv"

# ---------- use csv ----------
import csv

year = []
value = []

csvfile = open(FILE_NAME, "r")
plots = csv.reader(csvfile, delimiter=',')

for row in plots:
    year.append(int(row[0]))
    value.append(float(row[1]))

csvfile.close()

# customize X axis in the future
plt.bar(year, value, label="Loaded from File", color="red")

# ---------- use numpy ----------
import numpy as np

year, value = np.loadtxt(FILE_NAME, delimiter=',', unpack=True)
plt.scatter(year, value, label="Loaded from File", color="blue")

plt.xlabel("year")
plt.ylabel("value")
plt.title('Loaded from File')
plt.legend()
plt.show()  # show the drawing
