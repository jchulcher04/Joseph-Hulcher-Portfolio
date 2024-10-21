(print setTests[4T's])

(set null nil)
(set null2 null)
(set list ( 1 2 ))
(set list2 list)

(print (nil? null))
(print (nil? null2))
(print (= 1 (car list)))
(print (= 1 (car list2)))

./
setTests[4T's]
T
T
T
T