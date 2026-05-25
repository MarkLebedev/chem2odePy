    SUBROUTINE  FEX (NEQ, T, Y, YDOT)
    INTEGER  NEQ
    DOUBLE PRECISION  T, Y(3), YDOT(3)
    YDOT(1) = -.04D0*Y(1) + 1.D4*Y(2)*Y(3)
    YDOT(3) = 3.D7*Y(2)*Y(2)
    YDOT(2) = -YDOT(1) - YDOT(3)
    RETURN
    END

    SUBROUTINE  JEX (NEQ, T, Y, ML, MU, PD, NRPD)
    INTEGER  NEQ, ML, MU, NRPD
    DOUBLE PRECISION  T, Y(3), PD(NRPD,3)
    PD(1,1) = -.04D0
    PD(1,2) = 1.D4*Y(3)
    PD(1,3) = 1.D4*Y(2)
    PD(2,1) = .04D0
    PD(2,3) = -PD(1,3)
    PD(3,2) = 6.D7*Y(2)
    PD(2,2) = -PD(1,2) - PD(3,2)
    RETURN
    END


    PROGRAM TEST
    EXTERNAL FEX, JEX
    INTEGER  IOPT, IOUT, ISTATE, ITASK, ITOL, IWORK(23), LIW, LRW, MF, NEQ
    DOUBLE PRECISION  ATOL(3), RTOL, RWORK(58), T, TOUT, Y(3)
    NEQ = 3
    Y(1) = 1.D0
    Y(2) = 0.D0
    Y(3) = 0.D0
    T = 0.D0
    TOUT = .4D0
    ITOL = 2
    RTOL = 1.D-4
    ATOL(1) = 1.D-6
    ATOL(2) = 1.D-10
    ATOL(3) = 1.D-6
    ITASK = 1
    ISTATE = 1
    IOPT = 0
    LRW = 58
    LIW = 23
    MF = 21
    DO 40 IOUT = 1,12
      CALL DLSODE (FEX, NEQ, Y, T, TOUT, ITOL, RTOL, ATOL, ITASK, ISTATE, IOPT, RWORK, LRW, IWORK, LIW, JEX, MF)
      WRITE(6,20)  T, Y(1), Y(2), Y(3)
20    FORMAT(' At t =',D12.4,'   y =',3D14.6)
      IF (ISTATE .LT. 0)  GO TO 80
40    TOUT = TOUT*10.D0
    WRITE(6,60)  IWORK(11), IWORK(12), IWORK(13)
60  FORMAT(/' No. steps =',i4,',  No. f-s =',i4,',  No. J-s =',i4)
    STOP
80  WRITE(6,90)  ISTATE
90  FORMAT(///' Error halt.. ISTATE =',I3)
    STOP
    END PROGRAM

