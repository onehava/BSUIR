import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.Vector;

public class SwingUI {
    private JPanel rootPanel;
    private JTextField titleField;
    private JTextField priceField;
    private JTextField countField;
    private JButton insertButton;
    private JButton selectButton;
    private JButton maxTotalButton;
    private JButton aboveThePriceButton;
    private JButton aboveAvgPriceButton;
    private JButton containsCharacterButton;
    private JTable resultTable;

    public SwingUI(){
        this.resetResultTable();
    }

    public String getTitleText(){
        return titleField.getText();
    }

    public String getPriceText(){
        return priceField.getText();
    }

    public String getCountText(){
        return countField.getText();
    }

    public JButton getInsertButton(){
        return insertButton;
    }

    public JButton getSelectButton(){
        return selectButton;
    }

    public JButton getMaxTotalButton(){
        return maxTotalButton;
    }

    public JButton getAboveThePriceButton(){
        return aboveThePriceButton;
    }

    public JButton getAboveAvgPriceButton(){
        return aboveAvgPriceButton;
    }

    public JButton getContainsCharacterButton(){
        return containsCharacterButton;
    }

    public JPanel getRootPanel(){
        return rootPanel;
    }

    public void resetTextFields(){
        titleField.setText("");
        priceField.setText("");
        countField.setText("");
    }

    public void resetResultTable(){
        resultTable.setModel(new DefaultTableModel(null, new String[]{"product", "price", "count"}));
    }

    public void setResultTable(ResultSet rs) throws SQLException {
        ResultSetMetaData metaData = rs.getMetaData();

        Vector<String> columnNames = new Vector<>();

        int columnCount = metaData.getColumnCount();

        for(int column = 1; column <= columnCount; column++)
            columnNames.add(metaData.getColumnName(column));

        Vector<Vector<Object>> data = new Vector<>();

        while(rs.next()){
            Vector<Object> vector = new Vector<>();

            for(int columnIndex = 1; columnIndex <= columnCount; columnIndex++)
                vector.add(rs.getObject(columnIndex));

            data.add(vector);
        }

        resultTable.setModel(new DefaultTableModel(data, columnNames));
    }
}
