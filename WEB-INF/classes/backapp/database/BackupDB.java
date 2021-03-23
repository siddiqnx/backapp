package backapp.database;

import java.sql.*;
import java.time.LocalDateTime;
import java.util.*;

import backapp.bean.Backup;

public class BackupDB extends DB {
  public static String[] BACKUP_FIELDS = {
    "source",
    "destination",
    "timestamp"
  };

  public boolean createBackup(
    String source,
    String destination,
    LocalDateTime _timestamp
  ) {
    String query = DBQueries.CREATE_BACKUP(
      source,
      destination,
      _timestamp.toString()
    );
    
    try {
      PreparedStatement statement = connection.prepareStatement(query);
      
      Timestamp timestamp = Timestamp.valueOf(_timestamp);
      
      statement.setString(1, source);
      statement.setString(2, destination);
      statement.setTimestamp(3, timestamp);
      
      int numRecords = statement.executeUpdate();
      if(numRecords <= 0) {
        return false;
      }
    } catch(SQLException e) {
      e.printStackTrace();
    } finally {
      closeConnection();
    }

    return true;
  }
  
  public List<Backup> getAllBackups() {
    String query = DBQueries.GET_ALL_BACKUPS();
    List<Backup> backupList = new ArrayList<Backup>();

    try {
      Statement statement = connection.createStatement();

      ResultSet result = statement.executeQuery(query);

      while(result.next()) {
        int id = result.getInt("id");
        String source = result.getString("source");
        String destination = result.getString("destination");
        Timestamp _timestamp = result.getTimestamp("timestamp");

        LocalDateTime timestamp = _timestamp.toLocalDateTime();

        Backup backup = new Backup(id, source, destination, timestamp);
        backupList.add(backup);
      }

    } catch(SQLException e) {
      e.printStackTrace();
    } finally {
      closeConnection();      
    }

    return backupList;
  }

  public Backup getBackupFromId(int id) {
    String query = DBQueries.GET_BACKUP_INFO_FROM_ID(id);
    Backup backup = null;
    
    try {
      Statement statement = connection.createStatement();
      
      ResultSet result = statement.executeQuery(query);
      
      while(result.next()) {
        String source = result.getString("source");
        String destination = result.getString("destination");
        Timestamp _timestamp = result.getTimestamp("timestamp");

        LocalDateTime timestamp = _timestamp.toLocalDateTime();

        backup = new Backup(id, source, destination, timestamp);
      }
    } catch (SQLException e) {
      e.printStackTrace();
    } finally {
      closeConnection();
    }

    return backup;
  }
}