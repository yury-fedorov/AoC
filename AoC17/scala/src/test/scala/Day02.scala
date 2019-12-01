import org.scalatest._

class Day02 extends FlatSpec with Matchers {

  // minMaxSum
  def task02a(v:Seq[Int]): Int = v.max - v.min

	def readFile(filename: String): Seq[String] = {
	    val bufferedSource = io.Source.fromFile(filename)
	    val lines = (for (line <- bufferedSource.getLines()) yield line).toList
	    bufferedSource.close
	    lines
	}

	def task02b(v:Seq[Int]) : (Int,Int) = {
		val o = v.sorted;
		val cross = o.flatMap(x => o.map(y => (x, y)))
		cross.find{case (a, b) => a < b &&  b % a == 0}.getOrElse( sys.error("not expected") )
	}

	def stringToSeq(str:String, sep:String) : Seq[Int] =  str.split(sep).map( _.toInt)
  
  "Task 1 samples" should "pass" in {
 	task02a(stringToSeq("5 1 9 5", " ")) should be (8)
	task02a(stringToSeq("7 5 3", " ")) should be (4)
	task02a(stringToSeq("2 4 6 8", " ")) should be (6)
  }  

  "Task 2 samples" should "pass" in {
 	task02b(stringToSeq("5 9 2 8", " ")) should be (2,8)
 	task02b(stringToSeq("9 4 7 3", " ")) should be (3,9)
 	task02b(stringToSeq("3 8 6 5", " ")) should be (3,6)
  }  

  "Given task" should "pass" in {
	val lines = readFile("input02.txt")
	val numbers = lines.map( stringToSeq( _, "\t" ) )
	numbers.map( task02a ).sum should be (51833)  // response 1
	numbers.map( task02b ).map{case (a: Int, b: Int) => b/a}.sum should be (288) // response 2
  }
}
