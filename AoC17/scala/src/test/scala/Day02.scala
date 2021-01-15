import org.scalatest.funsuite.AnyFunSuite

class Day02 extends AnyFunSuite {

  // minMaxSum
  def task02a(v:Seq[Int]): Int = v.max - v.min

	def readFile(filename: String): Seq[String] = {
	    val bufferedSource = io.Source.fromResource(filename)
	    val lines = (for (line <- bufferedSource.getLines()) yield line).toList
	    bufferedSource.close
	    lines
	}

	def task02b(v:Seq[Int]) : (Int,Int) = {
		val o = v.sorted
		val cross = o.flatMap(x => o.map(y => (x, y)))
		cross.find{case (a, b) => a < b &&  b % a == 0}.getOrElse( sys.error("not expected") )
	}

	def stringToSeq(str:String, sep:String) : Seq[Int] =  str.split(sep).map( _.toInt)
  
  test( "Task 1 samples" ) {
 		assert( task02a(stringToSeq("5 1 9 5", " ")) == 8)
		assert( task02a(stringToSeq("7 5 3", " ")) == 4)
		assert( task02a(stringToSeq("2 4 6 8", " ")) == 6)
  }  

  test( "Task 2 samples" ) {
 		assert( task02b(stringToSeq("5 9 2 8", " ")) == (2,8) )
 		assert( task02b(stringToSeq("9 4 7 3", " ")) == (3,9) )
 		assert( task02b(stringToSeq("3 8 6 5", " ")) == (3,6) )
  }  

  test ( "Given task" ) {
		val lines = readFile("input02.txt")
		val numbers = lines.map( stringToSeq( _, "\t" ) )
		assert( numbers.map( task02a ).sum == 51833 ) // response 1
		assert( numbers.map( task02b ).map{case (a: Int, b: Int) => b/a}.sum == 288) // response 2
  }
}
