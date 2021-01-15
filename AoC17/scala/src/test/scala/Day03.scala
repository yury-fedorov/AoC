import org.scalatest.funsuite.AnyFunSuite

class Day03 extends AnyFunSuite {

  def d(n:Int) : Int = {
    if (n<=1) 0
    else {
      val exactSquare = Math.sqrt(n) // 1.4
      val ceilSquare_ : Int = Math.ceil(exactSquare).toInt // 2
      val correctedCeilSquare : Int = if ( ceilSquare_ % 2 == 1 ) ceilSquare_ else ceilSquare_ + 1 // 3
      val preRingEnd : Int = Math.pow(correctedCeilSquare-2,2).toInt // 1
      val curRingEnd : Int = Math.pow(correctedCeilSquare,2).toInt // 9
      val ringDiff : Int = curRingEnd - preRingEnd
      assert( ringDiff>0, "ring difference check failed" ) // every ring is greater then previous one
      val ringEight : Int = ringDiff / 8 
      assert( ringDiff % 8 == 0, "ring by 8 devision check failed" + n + " " + preRingEnd + " " + curRingEnd ) // any ring contains 8*n elements
      val fromStartOfRing : Int = n - preRingEnd - 1
      val ringFourth : Int = ringEight * 2
      val ringOffset : Int = fromStartOfRing % ringFourth // this is relevant part for distance
      val minRingDistance =  ( correctedCeilSquare - 1 ) / 2 // to get to the ring from the center
      var onRingDistance = 0
      if ( ringOffset != 0 ) {
        onRingDistance = ((correctedCeilSquare - 1)/2) - 1 // starting point
        var isGoAway : Boolean = onRingDistance == 0 // start with going closer 
        for ( _ <- 1 to ringOffset ) {
          if (!isGoAway) {
            onRingDistance = onRingDistance - 1
            isGoAway = !( onRingDistance > 0 )
          } else {
            onRingDistance = onRingDistance + 1
            isGoAway = onRingDistance < correctedCeilSquare
          }
        }        
      }
      minRingDistance + onRingDistance
    }    	
  }

  test( "samples" ) {
    assert( d(1) == 0 ) // Data from square 1 is carried 0 steps, since it's at the access port.
    assert( d(2) == 1)
    assert( d(12) == 3) // Data from square 12 is carried 3 steps, such as: down, left, left.
    assert( d(23) == 2) // Data from square 23 is carried only 2 steps: up twice.
    assert( d(1024) == 31) // Data from square 1024 must be carried 31 steps.
  }

  test( "solutions" ) {
    val n = 347991
    assert( d(n) == 480, "answer 1")
  }
}
