# Pie Chart
import matplotlib.pyplot as plt

slices = [7, 3, 9, 1, 4]
activities = ["Sleeping", "Eating", "Working", "Playing", "Running"]
colors = ["blue", "red", "cyan", "green", "yellow"]
# explode - pull one slice out from the chart, length must match label length
# autopct - show percentage ("%1.1f%%" no idea)
plt.pie(slices, labels=activities, colors=colors,
        startangle=90, shadow=True, explode=(0, 0.15, 0, 0, 0), autopct='%1.1f%%')

plt.title('Sample Pie Chart')
plt.show()  # show the drawing
