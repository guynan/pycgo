

package main

import "C"


//export GoisPrime
func GoisPrime(x int) int {

	if	x < 2 {
		return 0;
	}

	for i := 2; i <= x; i++ {

		if i == x {
			return 1;
		}

		if x % i == 0 {
			break;
		}
	}

	return 0;

}


func main() {
}
