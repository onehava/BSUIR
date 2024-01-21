import scala.io.StdIn
import java.util
import java.util.Locale

object L1 {
  private def splitString(number: Long): (Long, Long) = {
    val numberStr = number.toString
    val A: Long = numberStr.take(8).toLong
    val B: Long = numberStr.slice(8, 16).toLong
    (A, B)
  }

  def main(args: Array[String]): Unit = {
    print("Enter 16-digit number: ")
    var z = StdIn.readLong()

    if (z.toString.length != 16) {
      println("Incorrect input")
    } else {
      for (_ <- 1 to 5) {
        val (a, b) = splitString(z)
        val c = a * b
        val rez = c.toString.take(3).toFloat / 1000
        println(s"Rez: $rez")

        z = (z * c) % 10000000000000000L
        z = Math.abs(z)

        if(z == 0){
          z = 1
        }
        z = z.toString.padTo(16, '0').toLong
      }
    }
  }
}

class Generator(val seed: Long) {
  private var next: Double = seed

  def rand(): Double = {
    val a = 16807
    val m = 2147483647
    val c = 1
    next = (a * next + c) % m
    next / m
  }
}

class ProcentDistribution(val discretization: Int){
  private val counts = Array.fill(discretization)(0)

  def push(number: Double): Unit = {
    counts((number * discretization).toInt) += 1
  }

  def intervals(): Array[(Double, Double, Double)] = {
    val pInterval = 1.0 / discretization
    val pNumber = 100.0 / counts.sum

    for((n, i) <- counts.zipWithIndex)
      yield (i * pInterval, (i + 1) * pInterval, n * pNumber)
  }
}

class EntryDistribution(val discretization: Int){
  private val lists = Array.fill(discretization)(new util.ArrayList[Double])

  def push(number: Double): Unit = {
    lists((number * discretization).toInt).add(number)
  }

  def intervals(): Array[(Double, Double, util.ArrayList[Double])] = {
    val pInterval = 1.0 / discretization

    for((v, i) <- lists.zipWithIndex)
      yield  (i * pInterval, (i + 1) * pInterval, v)
  }
}

object L1A1 {
  def main(args: Array[String]): Unit = {
    val seed = 123
    val numbers = 10
    val discretization = 5

    val generator = new Generator(seed)
    val distribution = new EntryDistribution(discretization)

    for (_ <- 1 to numbers) {
      distribution.push(generator.rand())
    }

    for ((a, b, v) <- distribution.intervals()) {
      println(String.format(Locale.US, "%.2f - %.2f: %s", a, b, v))
    }
  }
}

object L1A2 {
  def main(args: Array[String]): Unit = {
    val seed = 123
    val numbers = 1_000_000
    val discretization = 20

    val generator = new Generator(seed)
    val distribution = new ProcentDistribution(discretization)

    for (_ <- 1 to numbers) {
      distribution.push(generator.rand())
    }

    for ((a, b, p) <- distribution.intervals()) {
      println(String.format(Locale.US, "%.2f - %.2f: %.1f%%", a, b, p))
    }
  }
}