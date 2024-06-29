package main

import "fmt"

func main() {
	var a int16 = 3456
	var b int8 = int8(a)
	fmt.Println(a, b)

	a = int16(b)
	fmt.Println(a, b)
}
