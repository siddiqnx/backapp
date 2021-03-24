package backapp.controller;

import java.io.*;
import java.util.*;

import javax.servlet.http.*;
import javax.servlet.annotation.WebServlet;
import javax.servlet.ServletException;

import backapp.database.BackupDB;

import backapp.nativelink.BackupC;
import backapp.bean.Backup;

public class RestoreBackup extends HttpServlet{
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
    
    boolean backupRestored = backupC.restoreBackup(
      filePath,
      backup.source,
      password
    );

    request.setAttribute("success", backupRestored);

    request.getRequestDispatcher("/backups").forward(request, response);
	}
}