import yaml

stream = open('chemisty_example.yaml', 'r')
chemistry = yaml.load(stream, Loader=yaml.Loader)

print(chemistry)



