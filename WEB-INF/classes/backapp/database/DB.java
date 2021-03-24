package backapp.database;

import java.util.*;
import java.io.*;
import java.sql.*;

import backapp.*;
import backapp.Config;

public class DB {
  protected Connection connection;

  public DB() {
    setConnection();
  }

  protected void setConnection() {
    try {
      Class.forName("org.postgresql.Driver");
      connection = DriverManager.getConnection(
        Config.DB_URL,
        Config.DB_USERNAME,
        Config.DB_PASSWORD
      );
    } catch(SQLException e) {
      System.out.println("Error connecting to DB");
      e.printStackTrace();
    } catch(ClassNotFoundException e) {
      e.printStackTrace();
    }
  }
  
  protected void closeConnection() {
    try {
      connection.close();
    } catch(SQLException e) {
      System.out.println("Error closing DB connection");
      e.printStackTrace();
    }
  }
}