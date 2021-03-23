package backapp.controller;

import java.io.*;
import java.util.*;

import javax.servlet.http.*;
import javax.servlet.annotation.WebServlet;
import javax.servlet.ServletException;

import backapp.database.BackupDB;

import backapp.bean.Backup;

public class RestoreBackup extends HttpServlet{
  protected void doGet(
    HttpServletRequest request,
    HttpServletResponse response
  ) throws ServletException, IOException {
    Integer backupId = Integer.parseInt(request.getParameter("id"));
    String password = (String) request.getParameter("password");
    BackupDB db = new BackupDB();

    Backup backupObj = db.getBackupFromId(backupId);

    backapp.nativelink.Backup backup = new backapp.nativelink.Backup();

    String[] strArray = backupObj.source.split("/");
    String backupFileName = strArray[strArray.length - 1];

    String filePath = backupObj.destination + "/" + backupFileName;

    System.out.println(filePath);
    
    boolean backupRestored = backup.restoreBackup(
      filePath,
      backupObj.source,
      password
    );

		response.sendRedirect(request.getContextPath() + "/backups");
	}
}