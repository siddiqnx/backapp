package backapp.nativelink;

public class BackupC {

  static {
    System.loadLibrary("backuplib");
  }

  public native boolean createBackup(String source, String destination, String backupFileName, String encryptionKey);

  public native boolean restoreBackup(String backupFilePath, String restorePath, String decryptionKey);
  
  public native boolean deleteBackup(String backupFilePath, String decryptionKey);
}