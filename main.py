import yaml

def power(text, power):
    output = ''
    for i in range(power):
        output += text
    return output

stream = open('chemistry.yaml', 'r')
chemistry = yaml.load(stream, Loader=yaml.Loader)

output_string = ""

for i in chemistry['species']:
    output_string += f'YDOT({i}) ='
    for r in range(len(chemistry['reactions'])):
        for reagent in chemistry['reactions'][r]['reagents']:
            if i in reagent:
                output_string += f' -{reagent.split('_')[0]}*{chemistry['reactions'][r]['rates'][0]}'
                for reagent in chemistry['reactions'][r]['reagents']:
                    output_string += power(f'*Y({reagent.split('_')[1]})', int(reagent.split('_')[0]))
                break
        for product in chemistry['reactions'][r]['products']:
            if i in product:
                output_string += f' +{product.split('_')[0]}*{chemistry['reactions'][r]['rates'][0]}'
                for reagent in chemistry['reactions'][r]['reagents']:
                    output_string += power(f'*Y({reagent.split('_')[1]})', int(reagent.split('_')[0]))
                break
        if chemistry['reactions'][r]['reversible']:
            temp_reagents = chemistry['reactions'][r]['products']
            temp_products = chemistry['reactions'][r]['reagents']
            for reagent in temp_reagents:
                if i in reagent:
                    output_string += f' -{reagent.split('_')[0]}*{chemistry['reactions'][r]['rates'][1]}'
                    for reagent in temp_reagents:
                        output_string += power(f'*Y({reagent.split('_')[1]})', int(reagent.split('_')[0]))
                    break
            for product in temp_products:
                if i in product:
                    output_string += f' +{product.split('_')[0]}*{chemistry['reactions'][r]['rates'][1]}'
                    for reagent in temp_reagents:
                        output_string += power(f'*Y({reagent.split('_')[1]})', int(reagent.split('_')[0]))
                    break

    output_string += '\n'

output_string = output_string[:-1]
with open('ode.txt', 'w') as f:
    f.write(output_string)

print('Done')