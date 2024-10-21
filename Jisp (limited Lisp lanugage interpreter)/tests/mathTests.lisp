(define FibonachiAdd (level) (cond
    (< level 1) (0)
    (< level 2) (1)
    (t) (+ (FibonachiAdd (- level 1)) (FibonachiAdd (- level 2)))
))

(print addingTests[6T's]:) ; AddingTests:
(print (= 0 (FibonachiAdd 0)) ) ; 
(print (= 1 (FibonachiAdd 1)))  ; 
(print (= 5 (FibonachiAdd 5)))  ; 
(print (= 55 (FibonachiAdd 10)) ) ; 
(print (= 610 (FibonachiAdd 15)))  ; 
(print (= 4181 (FibonachiAdd 19)))  ; 


(print ------ )
(print +-/=Tests[6T's]:)
(print (= 5 (* 5 (- (/ 34 17) 1))))
(print (= 1492 (- (* 5 (/ 1500 5)) 8)))
(print (= (car (5 4 1)) (- 8 (* 3 (/ 4 (car (cdr (5 4))))))))
(print (= 25 (* 5 5)))
(print (= 3 (/ 12 4)))
(print (= 4 (- 5 1)))



/*
addingTests[6T's]:
T
T
T
T
T
T
------
+-/=Tests[6T's]:
T
T
T
T
T
T



