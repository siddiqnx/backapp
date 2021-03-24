package backapp.controller;

import java.io.*;
import java.util.*;

import javax.servlet.http.*;
import javax.servlet.annotation.WebServlet;
import javax.servlet.ServletException;

import backapp.database.BackupDB;
import backapp.bean.Backup;

public class BackApp extends HttpServlet{
  protected void doGet(
    HttpServletRequest request,
    HttpServletResponse response
  ) throws ServletException, IOException {
    
  BackupDB db = new BackupDB();
  HttpSession session = request.getSession();
  
  List<Backup> backups = db.getAllBackups();

  request.setAttribute("backups", backups);
  
  request.getRequestDispatcher("index.jsp").forward(request, response);
	}
}