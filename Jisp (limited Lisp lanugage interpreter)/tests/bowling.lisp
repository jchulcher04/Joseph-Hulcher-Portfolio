(define ball1 (scores) (car scores))
(define ball2 (scores) (car (cdr scores)))
(define ball3 (scores) (car (cdr (cdr scores))))
(define rest1 (scores) (cdr scores))
(define rest2 (scores) (cdr (cdr scores)))

(define bowlingScorer (scores frames) (cond
    (< frames 1) 0 ;if no more frames
    (= (ball1 ! scores) 10) (+ (ball1 ! scores) (+ (ball2 ! scores) (+ (ball3 ! scores) (bowlingScorer (rest1 ! scores) (- frames 1))))) ;calculate for strike
    (= (+ (ball1 ! scores) (ball2 ! scores)) 10) (+ (ball1 ! scores) (+ (ball2 ! scores) (+ (ball3 ! scores) (+ (bowlingScorer (rest2 ! scores) (- frames 1)))))) ;computes regular frame with spare
    t  (+ (ball1 ! scores) (+ (ball2 ! scores) (bowlingScorer (rest2 ! scores) (- frames 1)))) ;regular frame
))

((define bowling (scores) (bowlingScorer ! scores 10)) ) ;to add in number of frames
(set list1 (1 2 3 4 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0))
(set list2 (5 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0))
(set list3 (10 10 10 10     10 10 10 10     10 10 10 10))
(set list4 (5 3 5 3  3 5 5 3  3 5 3 5  6 2 2 6  7 1 1 7  ))
(set list5 (0 0 0 0     0 0 0 0        0 0 0 0      0 0 0 0     0 0 10 5 4))
(set list6 (0 0 0 0     0 0 0 0        0 0 0 0      0 0 0 0     10 5 4))


(print smallFunctionTests[5T's]:)
(print (= 1 (ball1 ! list1)))
(print (= 2 (ball2 ! list1)))
(print (= 3(ball3 ! list1)))
(print (= 2 (ball1 (rest1 ! list1))))
(print (= 5 (ball3 (rest2 ! list1))))

(print wholeFunctionTests[5T's]:)
(print (= 8 (bowling ! list2))) ; 8
(print (= 300 (bowling ! list3))) ;; 300, 12 strikes
(print (= 80 (bowling ! list4)));; 80
(print (= 19 (bowling ! list5))) ;; 19, strike in 10 frame
(print (= 28 (bowling ! list6))) ;; 28, strike in 9th frame

/*

smallFunctionTests[5T's]:
T
T
T
T
T
wholeFunctionTests[5T's]:
T
T
T
T
T