package main

import "fmt"

func main() {
	a := 10
	msg := "Hello Variable"

	a = 20
	msg = "Good Morning"

	fmt.Println(msg, a)

	var x int = 3
	var y int
	var z = 4
	u := 5
	v := "Hello"
	w := 3.14

	fmt.Println(x, y, z, u, v, w)
	fmt.Println(float64(u) + w)

	var i complex64 = 1 + 2i
	fmt.Println(i)
}
