import yaml

def main():
    driver_template = """SUBROUTINE  FEX (NEQ, T, Y, YDOT)
INTEGER  NEQ
DOUBLE PRECISION  T, Y(!N), YDOT(!N)
!ODE SYSTEM
RETURN
END

SUBROUTINE  JEX (NEQ, T, Y, ML, MU, PD, NRPD)
INTEGER  NEQ, ML, MU, NRPD
DOUBLE PRECISION  T, Y(!N), PD(NRPD,!N)
!JACOBIAN
RETURN
END


PROGRAM TEST
EXTERNAL FEX, JEX
INTEGER  IOPT, IOUT, ISTATE, ITASK, ITOL, IWORK(!LIW), LIW, LRW, MF, NEQ
DOUBLE PRECISION  ATOL(!N), RTOL, RWORK(!LRW), T, TOUT, Y(!N)
!NEQ = N
!INITIALS
!START TIME
!END TIME
!ITOL
!RTOL
!ATOLS (N)
ITASK = 1
ISTATE = 1
IOPT = 0
LRW = !LRW
LIW = !LIW
MF = 21
DO 40 IOUT = 1,12
  CALL DLSODE (FEX, NEQ, Y, T, TOUT, ITOL, RTOL, ATOL, ITASK, ISTATE, IOPT, RWORK, LRW, IWORK, LIW, JEX, MF)
  WRITE(6,20)  T, !Y(N)
20    FORMAT(' At t =',D12.4,'   y =',!ND14.6)
  IF (ISTATE .LT. 0)  GO TO 80
40    TOUT = TOUT*10.D0
WRITE(6,60)  IWORK(11), IWORK(12), IWORK(13)
60  FORMAT(/' No. steps =',i4,',  No. f-s =',i4,',  No. J-s =',i4)
STOP
80  WRITE(6,90)  ISTATE
90  FORMAT(///' Error halt.. ISTATE =',I3)
STOP
END PROGRAM
    """

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