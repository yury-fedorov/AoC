package aoc

// Once the solution is migrated to Go 1.21 the routines implemented here can disappear.

// Go 1.21 is not yet available on github, in future cmp.Compare
func Compare[I int64 | int](a, b I) int { return Ifelse(a == b, 0, Ifelse(a < b, -1, 1)) }
