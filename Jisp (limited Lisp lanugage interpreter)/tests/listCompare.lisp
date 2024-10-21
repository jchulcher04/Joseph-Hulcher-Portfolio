(define listCompare (list1 list2) (cond
    (nil? (car list1)) (nil? (car list2))
    (nil? (car list2)) nil
    T (listCompare (cdr list1) (cdr list2))
    ))

(set list4 (1 2 3 4 5 alf u 4jp12 ))
(set list5 list4)
(set list3 (1 2 3))    

(print TestsOf:listCompare[4T's]:)
(print (listCompare ! ( 1 2 3) ! ( 1 2 3)))
(print (nil? (listCompare ! (1 2 3) ! (1 2 3 4 5))))
(print (listCompare ! list4 ! list5))
(print (nil? (listCompare ! list4 ! list3)))

/*
TestsOf:listCompare[4T's]:
T
T
T
T