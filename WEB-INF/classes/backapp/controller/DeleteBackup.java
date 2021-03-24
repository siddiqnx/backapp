package backapp.controller;

import java.io.*;
import java.util.*;
import java.time.LocalDateTime;

import javax.servlet.http.*;
import javax.servlet.annotation.WebServlet;
import javax.servlet.ServletException;

import backapp.nativelink.BackupC;
import backapp.database.BackupDB;
import backapp.bean.Backup;

public class DeleteBackup extends HttpServlet {
  protected void doGet(
    HttpServletRequest request,
    HttpServletResponse response
  ) throws ServletException, IOException {
    Integer backupId = Integer.parseInt(request.getParameter("id"));
    String password = (String) request.getParameter("password");
    
    BackupDB db = new BackupDB();
    Backup backup = db.getBackupFromId(backupId);

    BackupC backupC = new BackupC();

    String[] strArray = backup.source.split("/");
    String backupFileName = strArray[strArray.length - 1];

    String filePath = backup.destination + "/" + backupFileName;
    
    boolean backupDeleted = backupC.deleteBackup(
      filePath,
      password
    );

    if(backupDeleted) {
      db.deleteBackup(backupId);
    }

    request.setAttribute("success", backupDeleted);

    request.getRequestDispatcher("/backups").forward(request, response);
	}
}