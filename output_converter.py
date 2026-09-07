import pandas as pd
import matplotlib.pyplot as plt
import yaml

import plotly.express as px

def convert_to_float(number):
    first = number.split('D')[0]
    second = number.split('D')[1]
    number = 10**int(second) * float(first)
    return number

data = []


with open('a.out', 'r') as file:
    for line in file:
        data.append(line.strip('\n').replace("  ", " "))

stream = open('chemistry.yaml', 'r')
chemistry = yaml.load(stream, Loader=yaml.Loader)

data = data[:-2]
datalist = []

for item in data:
    datalist.append(item.split(" ")[8:])

for i in range(len(datalist)):
    for j in range(len(datalist[i])):
        datalist[i][j] = convert_to_float(datalist[i][j])

df = pd.DataFrame(datalist)

print(df)

print(chemistry['species'])

fig = px.line(df, line_shape="spline", labels=chemistry['species'])
fig.show()

# plt.plot(df)
# plt.show()