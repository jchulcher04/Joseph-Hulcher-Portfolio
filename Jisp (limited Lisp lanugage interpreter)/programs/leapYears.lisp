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

(define remainder (numerater diviser) (
    - numerater (* (/ numerater diviser) diviser)
))


(define prepareYear (year) (cond
    (= (/ (- year 1) 4) (/ year 4)) (prepareYear (- year 1)) 
    T year
))

(print (prepareYear 15))
;(print (prepareYear 14))
;(print (prepareYear 13))
;(print (prepareYear 12))
;(print (prepareYear 11))


(print (remainder 15 3))
(print (remainder 17 3))
(print (remainder 2000 100))

(print seperate:-=-=-=-=-=-=-=-)

(printList ( find20Leaps (prepareYear 2015 ) ! () 0))
(printList ( find20Leaps (prepareYear 1903 ) ! () 0))

(printList2 ( find20Leaps (prepareYear 2015 ) ! () 0))
(printList2 ( find20Leaps (prepareYear 1903 ) ! () 0))


; This program works, but u can see the output here:
/*
12
0
2
0
seperate:-=-=-=-=-=-=-=-
1936
1940
1944
1948
1952
1956
1960
1964
1968
1972
1976
1980
1984
1988
1992
1996
2000
2004
2008
2012
finished
1820
1824
1828
1832
1836
1840
1844
1848
1852
1856
1860
1864
1868
1872
1876
1880
1884
1888
1892
1896
finished