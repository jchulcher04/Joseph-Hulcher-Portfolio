
; this is an O(n^2) solution
(define repeated? (item list) (cond
    (nil? (car list)) nil
    (= item (car list)) T 
    T (repeated? item (cdr list))
))

(define duplicates? (list) (cond
    (nil? (car list)) nil
    (repeated? (car list) (cdr list)) T
    T (duplicates? (cdr list))
))

(print (nil? (repeated? 5 ! (1 2 3 4))))
(print (repeated? 5 ! (1 2 3 4 5)))

(print (nil? (duplicates? ! (1 2 3 4 5))))
(print (duplicates? ! (1 2 3 4 5 3)))

/*
T
T
T
T