; this is just a program of useful joelisp functions

( define printList ( list ) ( if ( = ( car list ) nil )
   ( print finished )
   ( print ( car list )  (printList ( cdr list )) )
 ) ) 

( define printList2 ( list ) (print (quote list)))


(define remainder (numerater diviser) (
    - numerater (* (/ numerater diviser) diviser)
))

(define listCompare (list1 list2) (cond
    (nil? (car list1)) (nil? (car list2))
    (nil? (car list2)) nil
    T (listCompare (cdr list1) (cdr list2))
    ))

    