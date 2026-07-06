import yaml
import random

def translate(a):
    f = a//26
    r = a%26
    newa = f'{'Z'*f}{chr(r+65)}'
    return newa

nspecies = 1000
nreactions = 1000

specieslist = [translate(i) for i in range(nspecies)]
reactionlist = []

for i in range(nreactions):
    reactionlist.append({
        "reagents": [f'{random.randint(1,3)}_{random.choice(specieslist)}' for _ in range(random.randint(1,3))],
        "products": [f'{random.randint(1,3)}_{random.choice(specieslist)}' for _ in range(random.randint(1,3))],
        "reversible": True,
        "rates": [f'{random.randint(1,3)}D{random.randint(1,3)}' for _ in range(2)]
    })

data = {"species": specieslist,
        "reactions": reactionlist}

stream = open('chemistry.yaml', 'w')
yaml.dump(data, stream)
print(yaml.dump(data))