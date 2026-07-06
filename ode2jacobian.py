from chem2ode import fmult
import re
import time


def main():
    ode = []

    with open('ode.txt', 'r') as file:
        for line in file:
            ode.append(line.strip('\n'))

    start_time = time.perf_counter()

    output_string = ''
    for i in range(len(ode)):
        for j in range(len(ode)):
            target = f'Y({j + 1})'
            line = ode[i].split(' ')
            output = ''
            for element in line:
                power = element.count(target)
                if power != 0:
                    if re.search(r'\d*D\d', element.split('*')[0]):
                        newcoeff = fmult(power, element.split('*')[0])
                        if newcoeff[0] != '-':
                            newcoeff = '+' + newcoeff
                        newelement = str(newcoeff) + element.replace(element.split('*')[0], '', 1).replace(f'*{target}',
                                                                                                           '', 1)
                    else:
                        newelement = element.replace(f'*{target}', '', 1).replace(element.split('*')[1],
                                                                                  element.split('*')[1] + f'*{power}')
                    output += newelement + ' '
            if len(output) != 0:
                output_string += f'PD({i + 1},{j + 1}) = {output}'
                output_string += '\n'

    end_time = time.perf_counter()

    execution_time = end_time - start_time

    with open('jacobian.txt', 'w') as f:
        f.write(output_string)

    print(f"ode2jacobian execution time: {execution_time:.4f} seconds")