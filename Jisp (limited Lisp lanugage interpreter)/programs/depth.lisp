(define depthFinder (place depth) (cond
    (nil? place) depth
    (nil? (list? place)) (depthFinder (cdr place) depth)
    T (if (< (depthFinder (car place) (+ depth 1))) (depthFinder (cdr place) depth)
            (depthFinder (car place) (+ depth 1))
            (depthFinder (cdr place) depth)
    
        )    
    )
)

(print (depthFinder ! (5 6) 1))

; not finished