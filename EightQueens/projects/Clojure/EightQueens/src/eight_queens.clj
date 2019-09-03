(ns eight-queens)

(defn put-ok? "检测对于已有的盘面coll，是否可以在x位置放置棋子"
  [coll x]
  (let [n (count coll)]
    (loop [i 0]
      (if (< i n)
        (if (or (= x (nth coll i))
                (= (- n i) (- (nth coll i) x))
                (= (- n i) (- x (nth coll i))))
          false
          (recur (inc i)))
        true))))

(defn permutation "获取n个元素满足检查条件check?的排列"
  [n check?]
  (if (< 0 n)
    (loop [result [] stack [] begin 0]
      (if-let [x (some (fn [i] (when (check? stack i) i)) (range begin n))]
        (let [s (conj stack x)]
          (if (= n (count s))
            (recur (conj result s) stack (inc x))
            (recur result s 0)))
        (if (empty? stack)
          result
          (recur result (pop stack) (inc (last stack))))))
    []))

(defn show "打印一个盘面"
  [eq-map]
  (let [line (vec (repeat (count eq-map) \-))]
    (doseq [i eq-map]
      (apply println (assoc line i \*)))
    (println "====================")))

(defn -main []
  (let [eq-maps (permutation 8 put-ok?)]
    (doseq [eq-map eq-maps] (show eq-map))
    (println "map count:" (count eq-maps))))
