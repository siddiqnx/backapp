package backapp.database;

import java.util.*;
import backapp.*;
import backapp.Config;
import java.util.stream.Collectors;

public class DBQueries {
  
  public static String CREATE_BACKUP(String ...values) {
    String backupFields = String.join(", ", BackupDB.BACKUP_FIELDS);
    String backupValues = String.join("', '", values);
    
    String fieldPlaceholders =
      Arrays.asList(values)
            .stream()
            .map(x -> "?")
            .collect(Collectors.joining(", ", "(", ")"));
    
    String query = 
      "INSERT INTO " +
      Config.DB_BACKUP_TABLE + " " +
      "(" + backupFields + ")" +
      "VALUES " +
      fieldPlaceholders;    

    return query;
  }

  public static String GET_ALL_BACKUPS() {
    String query =
      "SELECT * FROM " +
      Config.DB_BACKUP_TABLE;

    return query;
  }
  
  public static String GET_BACKUP_INFO_FROM_ID(int id) {
    String query =
      "SELECT * FROM " +
      Config.DB_BACKUP_TABLE + " " +
      "WHERE id=" + id;
      
      return query;
  }

}