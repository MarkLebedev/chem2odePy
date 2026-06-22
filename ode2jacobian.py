from chem2ode import *

ode = []

stream = open('chemistry.yaml', 'r')
chemistry = yaml.load(stream, Loader=yaml.Loader)

with open('ode.txt','r') as file:
    for line in file:
        ode.append(line.strip('\n'))

output_string = ''
for i in range(len(ode)):
    for j in range(len(ode)):
        target = f'Y({chemistry['species'][j]})'
        line = ode[i].split(' ')
        output = ''
        for element in line:
            power = element.count(target)
            if power != 0:
                if re.search(r'\d*D\d', element.split('*')[0]):
                    newcoeff = fmult(power, element.split('*')[0])
                    if newcoeff[0] != '-':
                        newcoeff = '+'+newcoeff
                    newelement = str(newcoeff) + element.replace(element.split('*')[0],'',1).replace(f'*{target}','',1)
                else:
                    newelement = element.replace(f'*{target}','',1).replace(element.split('*')[1], element.split('*')[1]+f'*{power}')
                output += newelement + ' '
        if len(output) != 0:
            output_string += f'PD({i+1},{j+1}) = {output}'
            output_string += '\n'

with open('jacobian.txt', 'w') as f:
    f.write(output_string)

print('Done')