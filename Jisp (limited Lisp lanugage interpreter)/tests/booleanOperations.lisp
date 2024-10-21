(print Tests:if,Cond,=,<,>,and?,or?)

(print ifCond[3T's]:)
(print (if T T nil))
(print (if nil nil T))
(print (cond 
    nil nil
    nil nil
    T T))

(print =<>{6T's})
(print (= 1 1))
(print (nil? (= 1 0)))
(print (< 1 2))
(print (nil? (< 2 1)))
(print (> 2 1))
(print (nil? (> 1 2)))

(print and?or?[8T's])
(print (nil? (and? nil nil)))
(print (nil? (and? nil T)))
(print (nil? (and? T nil)))
(print (and? T T))
(print (nil? (or? nil nil)))
(print (or? T nil))
(print (or? nil T))
(print (or? T T))

/*
Tests:if,Cond,=,<,>,and?,or?
ifCond[3T's]:
T
T
T
=<>{6T's}
T
T
T
T
T
T
and?or?[8T's]
T
T
T
T
T
T
T
T