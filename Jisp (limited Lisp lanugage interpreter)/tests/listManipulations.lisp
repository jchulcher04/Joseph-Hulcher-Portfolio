(set list1 (1 2 3))
(print Tests:Cond,Car,Cdr)
(print individualTests[4T's])
(print (= 1 (car list1)))
(print (= 3 (car (cdr( cdr list1)))))
(print (= 4 (car (cons 4 list1))))
(print (= 2 (car (cdr (cdr (cons 4 list1))))))


( define checkList ( list num ) ( cond 
( = ( car list ) nil ) ( print T )
(= (car list) num ) (checkList ( cdr list ) (+ num 1) )
t (Print F)
 ) ) 

(print ComboTests{2T's})
( checkList ( cdr (  1 2 3 4 5 6 7 8 9 10 11 ) ) 2 )
( print(= 3 (car ( cdr ( cdr ( cons 1 (2 3)))))))

/*
Tests:Cond,Car,Cdr
individualTests[4T's]
T
T
T
T
ComboTests{2T's}
T
T
