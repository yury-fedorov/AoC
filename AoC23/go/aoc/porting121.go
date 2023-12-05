package aoc

// Once the solution is migrated to Go 1.21 the routines implemented here can disappear.

// Go 1.20 doesn't have min yet, only in Go 1.21
func Min[I int64 | int](a, b I) I {
	if a < b {
		return a
	}
	return b
}

// Go 1.21 is not yet available on github actions (Dec 2023)
func Max[I int64 | int](a, b I) I {
	if a > b {
		return a
	}
	return b
}
