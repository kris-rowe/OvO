program target__parallel

    USE OMP_LIB


    implicit none
  
    
    DOUBLE PRECISION :: COUNTER = 0

    
    
    INTEGER :: num_threads
    

     
    
    !$OMP TARGET   MAP(TOFROM: COUNTER) 



    

    
    !$OMP PARALLEL 



    
    num_threads = omp_get_num_threads()
    

    

!$OMP ATOMIC UPDATE

counter =  counter +1./num_threads


 
     

    !$OMP END PARALLEL
     

    !$OMP END TARGET
    

    IF  ( ( ABS(COUNTER - 1) ) > 10*EPSILON(COUNTER) ) THEN
        write(*,*)  'Expected 1 Got', COUNTER
        call exit(1)
    ENDIF

end program target__parallel