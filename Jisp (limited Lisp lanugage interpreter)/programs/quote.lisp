(define find20Leaps (year leapYears numOfLeaps) (cond
    (= numOfLeaps 20) (leapYears)
    (nil? (= (remainder year 100) 0)) (find20Leaps (- year 4) (cons year leapYears) (+ numOfLeaps 1)) 
    (= (remainder year 400) 0) (find20Leaps (- year 4) (cons year leapYears) (+ numOfLeaps 1)) 
    T (find20Leaps (- year 4) ! leapYears numOfLeaps) 
))

 ( define printList ( list ) ( if ( = ( car list ) nil )
   ( print finished )
   ( print ( car list )  (printList ( cdr list )) )
 ) ) 

( define printList2 ( list ) (print (quote list)))
( define evalList2 ( list ) (quote list))
( define getListQuote ( list ) (eval (quote list)))

(define remainder (numerater diviser) (
    - numerater (* (/ numerater diviser) diviser)
))


(define prepareYear (year) (cond
    (= (/ (- year 1) 4) (/ year 4)) (prepareYear (- year 1)) 
    T year
))

(set toCheck ( find20Leaps (prepareYear 2015 ) ! () 0))


(print (= (getListQuote(( find20Leaps (prepareYear 2015 ) ! () 0) )) (quote (1936 1940 1944 1948 1952 1956 1960 1964 1968 1972 1976 1980 1984 1988 1992 1996 2000 2004 2008 2012))))

(print (getListQuote(( find20Leaps (prepareYear 2015 ) ! () 0) )))

/*
T
 1936 1940 1944 1948 1952 1956 1960 1964 1968 1972 1976 1980 1984 1988 1992 1996 2000 2004 2008 2012



