(define blackJack (cards) (blackJackCalculator ! cards 0 0))

(define blackJackCalculator (cards score aces) (cond
    (nil? cards) (if (AND? (> score 21) (> aces 0)) 
                    (blackJackCalculator ! cards  (- score 10) (- aces 1))
                    (score)
    )
    (> (car cards) 10) (blackJackCalculator (cdr cards) (+ score 10) aces) 
    (= (car cards) 1) (blackJackCalculator (cdr cards) (+ score 11) (+ aces 1))
    (t) ( blackJackCalculator (cdr cards) (+ score (car cards)) aces)
))

(print ( blackJack ! ( 10 ) ))
print helloo
(print (blackJack ! (1 2 3 4 1)))
(print (blackJack ! (10 10 10)))

; output
/*
10
helloo
21
30