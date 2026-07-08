import yaml

def main():
    with open('driver_template.f90', 'r') as file:
        driver_template = file.read()

    with open('ode.txt', 'r') as file:
        ode = file.read()

    with open('ode.txt', 'r') as file:
        neq = sum(1 for line in file)

    with open('jacobian.txt', 'r') as file:
        jacobian = file.read()

    with open('ode.txt', 'r') as file:
        ylist = [line.split(" ")[0].replace("YDOT", "Y") for line in file]

    stream = open('driver_config.yaml', 'r')
    config = yaml.load(stream, Loader=yaml.Loader)

    initials = ''
    for i in range(len(config['initial conditions'])):
        initials += f'Y({i+1}) = {config['initial conditions'][i]}\n'

    atols = ''
    for i in range(len(config['ATOLS'])):
        atols += f'ATOL({i+1}) = {config['ATOLS'][i]}\n'

    yline = ""
    for y in ylist:
        yline += f'{y}, '

    driver_template = driver_template.replace("!ODE SYSTEM", ode)
    driver_template = driver_template.replace("!JACOBIAN", jacobian.strip())
    driver_template = driver_template.replace("!NEQ = N", f'NEQ = {neq}')
    driver_template = driver_template.replace("!Y(N)", yline[:-2])
    driver_template = driver_template.replace("!N", str(neq))
    driver_template = driver_template.replace("!INITIALS", initials.strip())
    driver_template = driver_template.replace("!START TIME", f'T = {config['start time']}')
    driver_template = driver_template.replace("!END TIME", f'TOUT = {config['end time']}')
    driver_template = driver_template.replace("!ITOL", f'ITOL = {config['ITOL']}')
    driver_template = driver_template.replace("!RTOL", f'RTOL = {config['RTOL']}')
    driver_template = driver_template.replace("!ATOLS (N)", atols.strip())
    driver_template = driver_template.replace("!LRW", str(22 +  9*neq + neq**2))
    driver_template = driver_template.replace("!LIW", str(20 +  neq))

    with open('driver.f90', 'w') as file:
        file.write(driver_template)