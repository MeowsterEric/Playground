# Stack plots
import matplotlib.pyplot as plt

days = [1, 2, 3, 4, 5]

sleeping = [7, 6.5, 9, 11, 7]
eating = [2, 3, 5, 3, 3]
working = [9, 7.5, 6, 6, 6]
playing = [6, 7, 4, 4, 8]

# stackplot has no way to show legends, trick to show is to plot an empty graph
plt.plot([], [], label="Sleeping", color="blue", linewidth=5)
plt.plot([], [], label="Eating", color="red", linewidth=5)
plt.plot([], [], label="Working", color="cyan", linewidth=5)
plt.plot([], [], label="Playing", color="green", linewidth=5)

# parameter orders: x, ys
plt.stackplot(days, sleeping, eating, working, playing,
              colors=["blue", "red", "cyan", "green"])

plt.xlabel("x")
plt.ylabel("y")
plt.title('Sample Stack Plot Graph')
plt.legend()
plt.show()  # show the drawing
