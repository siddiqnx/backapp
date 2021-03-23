package backapp.controller;

import java.io.*;
import java.util.*;
import java.time.LocalDateTime;

import javax.servlet.http.*;
import javax.servlet.annotation.WebServlet;
import javax.servlet.ServletException;

import backapp.nativelink.Backup;
import backapp.database.BackupDB;

public class CreateBackup extends HttpServlet{
  protected void doPost(
    HttpServletRequest request,
    HttpServletResponse response
  ) throws ServletException, IOException {
    String _source = (String) request.getParameter("source");
    String _destination = (String) request.getParameter("destination");
    String encryptionKey = (String) request.getParameter("ekey");
    
    Backup backup = new Backup();
    
    String source = _source.replace("\\", "/");
    String destination = _destination.replace("\\", "/");
    
    String[] strArray = source.split("/");
    
    String backupFileName = strArray[strArray.length - 1];
    
    boolean backupCreated = backup.createBackup(
      source,
      destination,
      backupFileName,
      encryptionKey
    );

    LocalDateTime backupTimestamp = LocalDateTime.now();

    if(backupCreated) {
      BackupDB db = new BackupDB();

      db.createBackup(
        source,
        destination,
        backupTimestamp
      );
    }

		response.sendRedirect(request.getContextPath() + "/backups");
	}
}