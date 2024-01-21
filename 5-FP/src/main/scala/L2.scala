import scala.annotation.tailrec
import scala.collection.mutable.ListBuffer

object L2T1 {
  private def function(numbers: List[Int]): Int = {
    numbers
      .filter(n => Math.abs(n) <= 5)
      .sum
  }

  private def dummyFunction(numbers: List[Int]): Int = {
    @tailrec
    def sumWithFilter(seq: List[Int], rez: Int = 0): Int = {
      if(seq.isEmpty)
        rez
      else
        sumWithFilter(seq.tail, if(Math.abs(seq.head) <= 5) rez + seq.head else rez)
    }
    sumWithFilter(numbers)
  }

  def main(args: Array[String]): Unit = {
    val input = List(-9, -7, -5, -4, -3, -2, -1, 0, 1, 2)
    println("Short: " + function(input))
    println("Dummy: " + dummyFunction(input))
  }
}

object L2T2 {
  private def function(numbers: List[Int]): Int = {
    numbers
      .zipWithIndex
      .collect{ case (number, index) if index % 2 == 1 => number }
      .sum
  }

  private def dummyFunction(numbers: List[Int]): Int =  {
    @tailrec
    def sumOddIndexes(seq: List[Int], counter: Int = 0, rez: Int = 0): Int = {
      if(seq.isEmpty)
        rez
      else
        sumOddIndexes(seq.tail, counter + 1, if(counter % 2 == 1) rez + seq.head else rez)
    }
    sumOddIndexes(numbers)
  }

  def main(args: Array[String]): Unit = {
    val input = List(-1, 1, -2, 2, -3, 3, -4, 4, -5, 5)
    println("Short: " + function(input))
    println("Dummy: " + dummyFunction(input))
  }
}

object L2T3 {
  private def function(numbers: List[Int]): Int = {
    numbers
      .zipWithIndex
      .minBy { case(number, _) => Math.abs(number - numbers.sum / numbers.size)}
      ._2
  }

  private def dummyFunction(numbers: List[Int]): Int = {
    @tailrec
    def mean(seq: List[Int], sum: Int = 0, count: Int = 0): Int =  {
      if(seq.isEmpty)
        sum / count
      else
        mean(seq.tail, sum + seq.head, count + 1)
    }
    @tailrec
    def indexOfNearest(seq: List[Int], v: Int, rez: Int = 0, rez_dv: Int = Int.MaxValue, count: Int = 0): Int = {
      if(seq.isEmpty)
        rez
      else {
        val dv = Math.abs(v - seq.head)
        if(dv < rez_dv)
          indexOfNearest(seq.tail, v, count, dv, count + 1)
        else
          indexOfNearest(seq.tail, v, rez, rez_dv, count + 1)
      }
    }
    if(numbers.isEmpty)
      throw new UnsupportedOperationException()
    else
      indexOfNearest(numbers, mean(numbers))
  }

  def main(args: Array[String]): Unit = {
    val input = List(-200, -100, -50, -1, 0, 1, 50, 100, 200)
    println("Short: " + function(input))
    println("Dummy: " + dummyFunction(input))
  }
}

object L2T4 {
  private def function(numbers: List[Int]): Boolean = {
    numbers
      .toSet.intersect(numbers.map(n => n * n).toSet)
      .nonEmpty
  }

  private def dummyFunction(numbers: List[Int]): Boolean = {
    @tailrec
    def hasSquared(seq: List[Int], subseq: List[Int]): Boolean = {
      if(subseq.isEmpty)
        false
      else if(seq.contains(subseq.head * subseq.head))
        true
      else
        hasSquared(seq, subseq.tail)
    }
    hasSquared(numbers, numbers)
  }

  def main(args: Array[String]): Unit = {
    val input = List(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
    println("Short: " + function(input))
    println("Dummy: " + dummyFunction(input))
  }
}

object L2T5 {
  private def function(numbers: List[Int]): Boolean = {
    numbers
      .toSet.size >= 3
  }

  private def dummyFunction(numbers: List[Int]): Boolean = {
    @tailrec
    def toSet(seq: List[Int], set: ListBuffer[Int] = ListBuffer()): List[Int] = {
      if(seq.isEmpty)
        set.toList
      else if(set.contains(seq.head))
        toSet(seq.tail, set)
      else
        toSet(seq.tail, set.addOne(seq.head))
    }
    toSet(numbers).size >= 3
  }

  def main(args: Array[String]): Unit = {
    val input = List(1, 1, 1, 1, 1, 1, 1, 1, 2, 3)
    println("Short: " + function(input))
    println("Dummy: " + dummyFunction(input))
  }
}

object Merge {
  private def function(a: List[Int], b: List[Int]): List[Int] = {
    a:::b
  }

  private def dummyFunction(a: List[Int], b: List[Int]): List[Int] = {
    ListBuffer
      .empty
      .addAll(a)
      .addAll(b)
      .toList
  }

  def main(args: Array[String]): Unit = {
    val a = List(1, 2, 3, 4)
    val b = List(5, 6, 7, 8, 9)
    println("Short: " + function(a, b))
    println("Dummy: " + dummyFunction(a, b))
  }
}