# bar charts and histograms
import matplotlib.pyplot as plt

# x = ["sample1", "sample2", "sample3", "sample4", "sample5"] # not support string?
x1 = [1, 2, 3, 4, 5]  # 2 inputs must have equal length
y1 = [6, 7, 8, 2, 4]
plt.bar(x1, y1, label='Bar1')
x2 = [6, 7, 8, 9, 10]
y2 = [14, 3, 7, 8, 22]
plt.bar(x2, y2, label='Bar2', color='#AAAAAA')  # red, blue, cyan, hax color, etc

# legends titles and labels
plt.xlabel('the X label')
plt.ylabel('the Y label')
plt.title('Bar Chart Graph\nSubtitle here')

# ------------------------------

# example for histogram
population_ages = [8, 22, 54, 15, 73, 15, 10, 49, 33, 26, 19, 29, 12, 8, 36, 33, 72, 56, 27, 7, 11, 4]  # sample data
bins = [0, 20, 40, 60, 80]  # x axis range
plt.hist(population_ages, bins, histtype='bar', rwidth=0.8, label='Population ')

plt.xlabel('ID')
plt.ylabel('Age')
plt.title('Population Ages Graph')

plt.legend()  # show the legends
plt.show()  # show the drawing
