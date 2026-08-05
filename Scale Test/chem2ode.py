import yaml
import re
import time

def power(text, power):
    output = ''
    for i in range(power):
        output += text
    return output

def fmult(normalint, fortint):
    temp = float(fortint.split('D')[0])
    result = temp*int(normalint)
    result = str(result)
    result += "D"+fortint.split('D')[1]
    return result

def main():
    stream = open('chemistry.yaml', 'r')
    chemistry = yaml.load(stream, Loader=yaml.Loader)

    output_string = ""

    start_time = time.perf_counter()

    for i in chemistry['species']:
        output_string += f'YDOT({i}) ='
        for r in range(len(chemistry['reactions'])):
            for reagent in chemistry['reactions'][r]['reagents']:
                if i == reagent.split("_")[1]:
                    if re.search(r"\d*D\d*", chemistry['reactions'][r]['rates'][0]):
                        output_string += f' -{fmult(reagent.split('_')[0], chemistry['reactions'][r]['rates'][0])}'
                    else:
                        output_string += f' -{reagent.split('_')[0]}*{chemistry['reactions'][r]['rates'][0]}'
                    for reagent in chemistry['reactions'][r]['reagents']:
                        output_string += power(f'*Y({reagent.split('_')[1]})', int(reagent.split('_')[0]))
                    break
            for product in chemistry['reactions'][r]['products']:
                if i == product.split("_")[1]:
                    if re.search(r"\d*D\d*", chemistry['reactions'][r]['rates'][0]):
                        output_string += f' +{fmult(product.split('_')[0], chemistry['reactions'][r]['rates'][0])}'
                    else:
                        output_string += f' +{product.split('_')[0]}*{chemistry['reactions'][r]['rates'][0]}'
                    for reagent in chemistry['reactions'][r]['reagents']:
                        output_string += power(f'*Y({reagent.split('_')[1]})', int(reagent.split('_')[0]))
                    break
            if chemistry['reactions'][r]['reversible']:
                temp_reagents = chemistry['reactions'][r]['products']
                temp_products = chemistry['reactions'][r]['reagents']
                for reagent in temp_reagents:
                    if i == reagent.split("_")[1]:
                        if re.search(r"\d*D\d*", chemistry['reactions'][r]['rates'][1]):
                            output_string += f' -{fmult(reagent.split('_')[0], chemistry['reactions'][r]['rates'][1])}'
                        else:
                            output_string += f' -{reagent.split('_')[0]}*{chemistry['reactions'][r]['rates'][1]}'
                        for reagent in temp_reagents:
                            output_string += power(f'*Y({reagent.split('_')[1]})', int(reagent.split('_')[0]))
                        break
                for product in temp_products:
                    if i == product.split("_")[1]:
                        if re.search(r"\d*D\d*", chemistry['reactions'][r]['rates'][1]):
                            output_string += f' +{fmult(product.split('_')[0], chemistry['reactions'][r]['rates'][1])}'
                        else:
                            output_string += f' +{product.split('_')[0]}*{chemistry['reactions'][r]['rates'][1]}'
                        for reagent in temp_reagents:
                            output_string += power(f'*Y({reagent.split('_')[1]})', int(reagent.split('_')[0]))
                        break

        output_string += '\n'

    for i in range(len(chemistry['species'])):
        output_string = output_string.replace(f'Y({chemistry["species"][i]})', f'Y({i + 1})').replace(f'YDOT({chemistry["species"][i]})', f'YDOT({i + 1})')

    end_time = time.perf_counter()

    execution_time = end_time - start_time

    output_string = output_string[:-1]
    with open('ode.txt', 'w') as f:
        f.write(output_string)

    print(f"chem2ode execution time: {execution_time:.4f} seconds")