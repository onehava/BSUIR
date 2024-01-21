import scala.io.StdIn
import scala.util.matching.Regex

object L3T1 {
  def main(args: Array[String]): Unit = {
    val target = "Hello to everybody and today we talk about java spring boot"

    val map = Map(
      'a' -> 'а', 'b' -> 'б', 'c' -> 'с', 'd' -> 'д', 'e' -> 'и',
      'f' -> 'ф', 'g' -> 'г', 'h' -> 'х', 'i' -> 'и', 'j' -> 'ж',
      'k' -> 'к', 'l' -> 'л', 'm' -> 'м', 'n' -> 'н', 'o' -> 'о',
      'p' -> 'п', 'q' -> 'й', 'r' -> 'р', 's' -> 'с', 't' -> 'т',
      'u' -> 'ю', 'v' -> 'в', 'w' -> 'в', 'x' -> 'х', 'y' -> 'у',
      'z' -> 'з'
    )

    val regexp = new Regex(".")

    println(
      regexp.replaceAllIn(target, data => {
        val matched = data.matched.head
        val replace = map.getOrElse(matched.toLower, matched)
        (if(matched.isUpper) replace.toUpper else replace).toString
      })
    )
  }
}

object L3T2 {
  def main(args: Array[String]): Unit = {
    val target = "When executing the exercise extract all extra words"
    val regexp = new Regex("ext[A-Za-z]+")

    regexp
      .findAllMatchIn(target)
      .foreach(w => println(w))
  }
}

object L3T3 {
  def main(args: Array[String]): Unit = {
    val target = "A big round ball fall to the ground"
    val regexp = new Regex("the\\b")

    println(
      regexp.replaceAllIn(target, "a")
    )
  }
}

object L3T4 {
  def main(args: Array[String]): Unit = {
    val target = StdIn.readLine()
    val regexp = new Regex("\\w+")

    println(regexp.replaceAllIn(target, word => word.matched.reverse))
  }
}

object L3T5 {
  def main(args: Array[String]): Unit = {
    val target = "Hello to everybody"
    val regexp = new Regex("[aeiouy]")

    println(regexp.replaceAllIn(target, ""))
  }
}

object L3T6 {
  def main(args: Array[String]): Unit = {
    val target = "Hello to everybody"
    val regexp = new Regex("[A-Za-z]")

    println(regexp.replaceAllIn(target, letter => letter.matched * 2))
  }
}

object L3T7 {
  def main(args: Array[String]): Unit = {
    val target = "Hello to everybody"
    val regexp = new Regex("y|Y")

    println(regexp.replaceAllIn(target, ""))
  }
}

object L3T8 {
  def main(args: Array[String]): Unit = {
    val target = "Hello to everybody"
    val regexp = new Regex("Hello\\b")

    println(regexp.replaceFirstIn(target, "Hello with heartness"))
  }
}

object L3A1 {
  def main(args: Array[String]): Unit = {
    val target = "Today is sunny day"
    val regexp = new Regex("(^|\\b)\\w")

    println(regexp.replaceAllIn(target, letter => letter.matched * 3))
  }
}

object L3A2 {
  def main(args: Array[String]): Unit = {
    val target = "Today is sunny day"
    val regexp1 = new Regex("\\W")
    val regexp2 = new Regex("\\s")

    println(regexp1.replaceAllIn(target, ""))
    println(regexp2.replaceAllIn(target, ""))
  }
}