import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

a = 5

# Считываем данные из файла
data = pd.read_csv('points.csv', sep=',', names=['x', 'y', 'isIn'], header=0)

# Разделяем точки на те, что попали в область, и те, что нет
inside = data[data['isIn'] == True]
outside = data[data['isIn'] == False]

# Рисуем точки
plt.scatter(inside['x'], inside['y'], color='green', alpha=0.6, s=1)
plt.scatter(outside['x'], outside['y'], color='gray', alpha=0.4, s=1)

# Рисуем лемнискату Бернулли
phi = np.linspace(-np.pi/4, np.pi/4, 1000)
r = np.sqrt(2*a**2 * np.cos(2*phi))
x = r * np.cos(phi)
all_x = np.concatenate((x, -x))
y = r * np.sin(phi)
all_y = np.concatenate((y, -y))
plt.plot(all_x, all_y, color='red')

# Рисуем окружность радиуса a
phi = np.linspace(-np.pi, np.pi, 1000)
circle_x = a * np.cos(phi)
circle_y = a * np.sin(phi)
plt.plot(circle_x, circle_y, color='black')

#plt.legend()
plt.savefig('area.jpg', dpi=600)