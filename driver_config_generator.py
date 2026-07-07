import yaml

config = {}

stream = open('chemistry.yaml', 'r')
chemistry = yaml.load(stream, Loader=yaml.Loader)

config["initial conditions"] = []
for i in chemistry["species"]:
    config["initial conditions"].append("1D0")

config['start time'] = '0D0'
config['end time'] = '1D0'
config['ITOL'] = 2
config['RTOL'] = '1.D-4'
config['ATOLS'] = []
for i in chemistry["species"]:
    config["ATOLS"].append("1D-6")

stream = open('driver_config.yaml', 'w')
yaml.dump(config, stream)

comment = """
#initial conditions - initial reagent abundances
#ITOL - 1 or 2 according as ATOL (below) is a scalar or an array
#RTOL - relative tolerance
#ATOL - absolute tolerance
#
#initial conditions, start time, RTOL, ATOL and end time in fortran-style floats (nDm = n*10^m - double precision /e.g 1.3D3/)
#defaul paramerters are included
"""

with open('driver_config.yaml', 'a') as config:
    config.write(comment)