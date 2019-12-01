import org.scalatest._

class Day03 extends FlatSpec with Matchers {

  def d(n:Int) : Int = {
    if (n<=1) return 0
    else {
      val exactSquare = Math.sqrt(n) // 1.4
      val ceilSquare_ : Int = Math.ceil(exactSquare).toInt // 2
      val correctedCeilSquare : Int = if ( ceilSquare_ % 2 == 1 ) ceilSquare_ else ( ceilSquare_ + 1 ) // 3
      val preRingEnd : Int = Math.pow(correctedCeilSquare-2,2).toInt // 1
      val curRingEnd : Int = Math.pow(correctedCeilSquare,2).toInt // 9
      val ringDiff : Int = curRingEnd - preRingEnd
      areEqual(true,ringDiff>0,"ring difference check failed" ) // every ring is greater then previous one
      val ringEight : Int = ringDiff / 8 
      areEqual(0, ringDiff % 8, "ring by 8 devision check failed" + n + " " + preRingEnd + " " + curRingEnd ) // any ring contains 8*n elements
      val fromStartOfRing : Int = n - preRingEnd - 1
      val ringFourth : Int = ringEight * 2
      val ringOffset : Int = fromStartOfRing % ringFourth // this is relevant part for distance
      val minRingDistance = ( ( correctedCeilSquare - 1 ) / 2 ).toInt // to get to the ring from the center
      var onRingDistance : Int = 0
      if ( ringOffset != 0 ) {
        onRingDistance = ((correctedCeilSquare - 1)/2) - 1 // starting point
        var isGoAway : Boolean = onRingDistance == 0 // start with going closer 
        for (  x <- 1 to ringOffset ) {
          if (isGoAway == false) {
            onRingDistance = onRingDistance - 1
            isGoAway = !( onRingDistance > 0 )
          } else {
            onRingDistance = onRingDistance + 1
            isGoAway = ( onRingDistance < correctedCeilSquare )
          }
        }        
      }
      /*
      System.out.println("--> for n:" + n)
      System.out.println("from start:" + fromStartOfRing)
      System.out.println("ring offset:" + ringOffset)
      System.out.println("min ring dist:" + minRingDistance)
      System.out.println("on ring dist:" + onRingDistance)
      */
      return minRingDistance + onRingDistance       
    }    	
  }

  "Day 3 tests" should "pass" in {  
    /*
	Data from square 1 is carried 0 steps, since it's at the access port.
	Data from square 12 is carried 3 steps, such as: down, left, left.
	Data from square 23 is carried only 2 steps: up twice.
	Data from square 1024 must be carried 31 steps.
    */
    d(1) should be (0)
    d(2) should be (1)
    d(12) should be (3)
    d(23) should be (2)
    d(1024) should be (31)
	  
    val n = 347991
    d(n) should be (480) // result 1
  }
}
