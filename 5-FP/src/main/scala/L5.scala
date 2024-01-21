import java.awt.event.ActionEvent
import java.sql.{DriverManager, ResultSet, SQLException}
import javax.swing.JOptionPane.showMessageDialog
import javax.swing.{JFrame, JOptionPane, SwingUtilities, UIManager, WindowConstants}

import com.mysql.cj.jdbc.Driver

class Database @throws[SQLException]()(url: String, username: String, password: String){
  Class.forName("com.mysql.cj.jdbc.Driver")
  private val connection = DriverManager.getConnection(url, username, password)

  def insert(product: String, price: Int, count: Int): Unit = {
    val statement = connection.createStatement()
    statement.execute(s"INSERT IGNORE INTO storage VALUES ('$product', $price, $count)")
  }

  def select(product: String): ResultSet = {
    val statement = connection.createStatement()
    statement.executeQuery(s"SELECT * FROM storage WHERE product = '$product'")
  }

  def maxByTotal(): ResultSet = {
    val statement = connection.createStatement()
    statement.executeQuery("SELECT * FROM storage WHERE price * count = (SELECT MAX(price * count) FROM storage)")
  }

  def aboveThePrice(price: Int): ResultSet = {
    val statement = connection.createStatement()
    statement.executeQuery(s"SELECT * FROM storage WHERE price > $price")
  }

  def aboveAvgPrice(price: Int): ResultSet = {
    val statement = connection.createStatement()
    statement.executeQuery(s"SELECT product, COUNT(product) AS records, AVG(price) AS average_price FROM storage GROUP BY product HAVING AVG(price) > $price")
  }

  def containsCharacter(): ResultSet = {
    val statement = connection.createStatement()
    statement.executeQuery("SELECT * FROM storage WHERE product LIKE '%a%'")
  }
}

object L5 {
  def main(args: Array[String]): Unit = {
    try {
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName)
    }
    catch {
      case _: Throwable => System.err.println("Failed to set system look and feel")
    }

    val link = "jdbc:mysql://localhost:3306/mydb"
    val root = "root"
    val pass = "pass"

    try {
      val database = new Database(link, root, pass)
      SwingUtilities.invokeLater(() => createUI(database))
    }
    catch {
      case _: SQLException => createErrorBox(s"Connection failed to '$link'.\n" +
                                              "Please, check the database accessibility and try again.")
    }
  }

  private def createErrorBox(message: String): Unit = {
    showMessageDialog(null, message, "Error", JOptionPane.ERROR_MESSAGE)
  }

  private def createUI(database: Database): Unit = {
    val ui = new SwingUI
    val root = ui.getRootPanel

    def title(): String = { ui.getTitleText }
    def price(): Int    = { ui.getPriceText.toInt }
    def count(): Int    = { ui.getCountText.toInt }

    def execute(action: () => Unit): Unit = {
      try {
        action()
      }
      catch {
        case _: SQLException => System.err.println("Unexpected SQLException (access error)")
        case _: NumberFormatException => createErrorBox("Wrong numeric input")
      }
      finally {
        ui.resetTextFields()
      }
    }

    ui.getInsertButton.addActionListener((_: ActionEvent) => {
      execute(() => {
        database.insert(title(), price(), count())
        ui.resetResultTable()
      })
    })

    ui.getSelectButton.addActionListener((_: ActionEvent) => {
      execute(() => {
        val rs = database.select(title())
        ui.setResultTable(rs)
      })
    })

    ui.getMaxTotalButton.addActionListener((_: ActionEvent) => {
      execute(() => {
        val rs = database.maxByTotal()
        ui.setResultTable(rs)
      })
    })

    ui.getAboveThePriceButton.addActionListener((_: ActionEvent) => {
      execute(() => {
        val rs = database.aboveThePrice(price())
        ui.setResultTable(rs)
      })
    })

    ui.getAboveAvgPriceButton.addActionListener((_: ActionEvent) => {
      execute(() => {
        val rs = database.aboveAvgPrice(price())
        ui.setResultTable(rs)
      })
    })

    ui.getContainsCharacterButton.addActionListener((_: ActionEvent) => {
      execute(() => {
        val rs = database.containsCharacter()
        ui.setResultTable(rs)
      })
    })

    val frame = new JFrame

    frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE)
    frame.setContentPane(root)
    frame.pack()
    frame.setLocationRelativeTo(null)
    frame.setVisible(true)
  }
}