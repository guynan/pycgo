

package main

/*
struct unsafearray {
	void* data;
	int length;
};
*/
import "C"
import "math"
//import "unsafe"
import "fmt"

type UnsafeArray C.struct_unsafearray;

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

//export parseArray
func parseArray(k []int64) {

	fmt.Println(k);

}


//export returnArray
func returnArray() UnsafeArray {

	var res UnsafeArray;

	primes := []byte("hello there");

	res.data = C.CBytes(primes);
	res.length = C.int(len(primes));

	return res
}

func main() {
}

