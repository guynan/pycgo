

package main

import "C"
import "math"


//export GoisPrime
func GoisPrime(x int) int {

	if	x < 2 {
		return 0;
	}

	ceil := int(math.Floor(math.Sqrt(float64(x)))) + 1

	for i := 2; i <= ceil; i++ {

		if i == ceil{
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
