import org.apache.spark.sql.{Dataset, SparkSession}

import java.io.File

object L4 {
  private def prepareArgument(args: Array[String]): String = {
    if(args.length < 1){
      System.err.println("Missing <input> path argument")
      System.exit(1)
    }

    if(!new File(args(0)).exists){
      System.err.println("Dataset <input> is not found")
      System.exit(1)
    }

    args(0)
  }

  @inline
  private def printFormat(set: Array[String]): String = {
    set.mkString(", ")
  }

  @inline
  private def printFormat(set: Dataset[String]): String = {
    printFormat(set.collect)
  }

  def main(args: Array[String]): Unit = {
    val source = prepareArgument(args)

    val spark = SparkSession
      .builder()
      .master("local[*]")
      .appName("Playing with Spark at BSUIR")
      .getOrCreate()

    spark.sparkContext.setLogLevel("ERROR")

    import spark.implicits._

    val lines = spark.read.textFile(source)

    val words = lines
      .flatMap(line => line.split("\\W+"))
      .map(word => word.toLowerCase)

    val stopWords = Set("without", "parties")

    val noStops = words.filter(word => !stopWords.contains(word))
    val tWords = words.filter(word => word.startsWith("t"))
    val ingWords = words.filter(word => word.endsWith("ing"))
    val aWords = words.filter(word => word.matches("(^|\\b)\\wa\\w+"))
    val sWords = words.filter(word => word.endsWith("s"))

    val evenWords = words.collect.grouped(2).map(_.head).toArray

    println(s"No stop-words: ${printFormat(noStops)}")
    println(s"Words starting with 't': ${printFormat(tWords)}")
    println(s"Words ending with 'ing': ${printFormat(ingWords)}")
    println(s"Words with 'a' as the second letter: ${printFormat(aWords)}")
    println(s"Words ending with 's': ${printFormat(sWords)}")
    println(s"Every second word: ${printFormat(evenWords)}")
  }
}

object L4A1 {
  def main(args: Array[String]): Unit = {
    val text =
      "Letter wooded direct two men indeed income sister.\n" +
      "Impression up admiration he by partiality is."

    val replaced1 = text.replaceAll("\\b\\w{1,3}", "ing")
    val replaced2 = text.replaceAll("\\w{1,3}\\b", "ing")

    println(s"Replace first 1-3 letters with 'ing':\n\n$replaced1\n")
    println(s"Replace last 1-3 letters with 'ing':\n\n$replaced2\n")
  }
}

object L4A2 {
  def main(args: Array[String]): Unit = {
    val text =
      "Old blushes respect but offices hearted minutes effects.\n" +
      "Written parties winding oh as in without on started."

    val replaced1 = text.replaceAll("[aeiouAEIOU]", "")
    val replaced2 = text.replaceAll("([^aeiouyAEIOUY0-9\\W]+)", "")

    println(s"Delete all vowels:\n\n$replaced1\n")
    println(s"Delete all consonants:\n\n$replaced2")
  }
}