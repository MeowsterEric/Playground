# Scatter Plots
import matplotlib.pyplot as plt

x = [1, 2, 3, 4, 5, 6, 7, 8]
y = [4, 6, 8, 3, 6, 9, 2, 4]
# marker - the dot symbol; s - size of the point
plt.scatter(x, y, label="legend", color="blue", marker="*", s=100)
# for full marker documentation, visit link below
# http://matplotlib.org/api/markers_api.html

plt.xlabel('x')
plt.ylabel('y')
plt.title('Sample Scatter Plot Graph')

plt.legend()  # show the legends
plt.show()  # show the drawing
