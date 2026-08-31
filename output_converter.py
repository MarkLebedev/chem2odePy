import pandas as pd
import matplotlib.pyplot as plt

def convert_to_float(number):
    first = number.split('D')[0]
    second = number.split('D')[1]
    number = 10**int(second) * float(first)
    return number

data = []


with open('a.out', 'r') as file:
    for line in file:
        data.append(line.strip('\n').replace("  ", " "))

data = data[:-2]
datalist = []

for item in data:
    datalist.append(item.split(" ")[8:])

for i in range(len(datalist)):
    for j in range(len(datalist[i])):
        datalist[i][j] = convert_to_float(datalist[i][j])

df = pd.DataFrame(datalist)

print(df)

plt.plot(df)
plt.show()