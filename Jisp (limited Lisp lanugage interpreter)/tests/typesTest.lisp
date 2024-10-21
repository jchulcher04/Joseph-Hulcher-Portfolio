(print NowTesting:number?[4T's])
(print ( number? 5))
(print ( number? 123456345345))
(print (nil? (number? 133g123))) ; checks that it isn't a number properly
(print (number? 5))

(print NowTesting:symbol?[4T's])
(print (nil? (symbol? sexy))) ; checks for that it detexts when not a symbol properly
(set sexy oolala)
(print (symbol? sexy))
(define localChecker (arg1) (symbol? arg1))
(print (symbol? localChecker))
(print(localChecker 5))


(print NowTesting:list?[6T's])

(set aList ( 1 2 3 ))
(print (list? aList))
(print (list? ( 1 2 3)))
(print (nil? (list? localChecker)))
(print (nil? (list? sexy)))
(print (nil? (list? hello)))

(define checkLocalList ( mrList) (list? mrList))
(print (checkLocalList ! ( hi)))

;Output
/*
NowTesting:number?[4T's]
T 
T 
T 
T 
NowTesting:symbol?[4T's]
T 
T 
T 
T 
NowTesting:list?[6T's]
T 
T 
T 
T 
T 
T 


