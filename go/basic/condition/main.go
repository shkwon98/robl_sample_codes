package main

import (
	"fmt"
	"os"
)

func main() {
	args := os.Args

	if len(args) != 2 {
		println("Please provide an argument!")
	} else {
		echo := args[1]
		fmt.Println("Echo:", echo)
	}
}
