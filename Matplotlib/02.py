# legends titles and labels
import matplotlib.pyplot as plt

x1 = [1, 4]  # X range
y1 = [8, 3]  # Y range
plt.plot(x1, y1, label='First Line')  # draw in the background

x2 = [4, 8]
y2 = [8, 12]
plt.plot(x2, y2, label='Second Line') # legend uses 'label' parameter

plt.xlabel('the X label')
plt.ylabel('the Y label')
plt.title('Demo Graph\nSubtitle here')

plt.legend() # show the legends
plt.show()  # show the drawing
