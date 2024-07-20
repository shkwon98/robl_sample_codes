package main

import "fmt"

func main() {
	// case 1: 명시적 타입 선언
	var var1 string

	var1 = "Hello Var1"
	fmt.Println(var1)

	var1 = "Good Morning, Var1"
	fmt.Println(var1)

	// case 2: 명시적 타입 선언과 초기화
	var var2 string = "Hello Var2"
	fmt.Println(var2)
	var2 = "Good Morning, Var2"
	fmt.Println(var2)

	// case 3: 타입 추론
	var var3 = "Hello Var3"
	fmt.Println(var3)
	var3 = "Good Morning, Var3"
	fmt.Println(var3)

	// case 4: 타입 추론(짧은 선언)
	var4 := "Hello Var4"
	fmt.Println(var4)
	var4 = "Good Morning, Var4"
	fmt.Println(var4)

	x := 5
	y := 3.14
	fmt.Println(float64(x) + y)

	i := 1 + 2i
	fmt.Println(i)
}
