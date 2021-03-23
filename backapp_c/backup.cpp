#include <iostream>
#include "include/backapp_nativelink_Backup.h"
#include "include/CkZip.h"
#include "include/CkDirTree.h"
#include "include/CkFileAccess.h"
#include "include/CkZipEntry.h"
#include "include/CkDateTime.h"

jboolean Java_backapp_nativelink_Backup_createBackup(
  JNIEnv *env,
  jobject obj,
  jstring jsource, 
  jstring jdestination, 
  jstring jbackupFileName, 
  jstring jencryptionKey
) {
  CkZip zip;
  CkFileAccess fileAccess;
  const char *convertedValue;
  
  const int AES_CODE = 4;
	const int AES_ENCRYPTION_LENGTH = 128;
  bool success = false;

  // Converting all jstring values to strings
  jboolean isCopy;
  convertedValue = (env)->GetStringUTFChars(jsource, &isCopy);
  std::string source = std::string(convertedValue);
  
  convertedValue = (env)->GetStringUTFChars(jdestination, &isCopy);
  std::string destination = std::string(convertedValue);
  
  convertedValue = (env)->GetStringUTFChars(jbackupFileName, &isCopy);
  std::string backupFileName = std::string(convertedValue);
  
  convertedValue = (env)->GetStringUTFChars(jencryptionKey, &isCopy);
  std::string encryptionKey = std::string(convertedValue);

  std::string backupFileFullPath = destination + "/" + backupFileName;

  bool backupExists = fileAccess.FileExists(backupFileFullPath.c_str());

  success = zip.NewZip(backupFileFullPath.c_str());
  if (!success) {
      std::cout << zip.lastErrorText() << "\r\n";
      return false;
  }

  zip.AppendFiles((source + "/*").c_str(), true);

	zip.put_Encryption(AES_CODE);
	zip.put_EncryptKeyLength(AES_ENCRYPTION_LENGTH);
	zip.put_EncryptPassword(encryptionKey.c_str());

  success = zip.WriteZipAndClose();

  if (!success) {
    std::cout << zip.lastErrorText() << "\r\n";
    return false;
  }
  zip.CloseZip();
	return true;
}

jboolean Java_backapp_nativelink_Backup_restoreBackup(
  JNIEnv *env,
  jobject obj,
  jstring jbackupFilePath,
  jstring jrestorePath,
  jstring jdecryptionKey
) {
  CkFileAccess fileAccess;
  CkZip zip;
  CkZipEntry *entry, *nextEntry;  
  const char *convertedValue;

  // Converting all jstring values to strings
  jboolean isCopy;
  convertedValue = (env)->GetStringUTFChars(jbackupFilePath, &isCopy);
  std::string backupFilePath = std::string(convertedValue);
  
  convertedValue = (env)->GetStringUTFChars(jrestorePath, &isCopy);
  std::string restorePath = std::string(convertedValue);
  
  convertedValue = (env)->GetStringUTFChars(jdecryptionKey, &isCopy);
  std::string decryptionKey = std::string(convertedValue);

  bool backupFileExists = fileAccess.FileExists(backupFilePath.c_str());
  
  if(!backupFileExists) {
    return false;
  }

  zip.OpenZip(backupFilePath.c_str());
  
  zip.put_DecryptPassword(decryptionKey.c_str());

  bool isPasswordOk = zip.VerifyPassword();

  if(!isPasswordOk) {
    return false;
  }
  

  bool restorePathExists = fileAccess.FileExists(restorePath.c_str());

  if(!restorePathExists) {
    // If restore directory doesn't exist, a new restore directory is created
    // and all the files are unzipped there without iterating over files.
    fileAccess.DirEnsureExists(restorePath.c_str());
    zip.Unzip(restorePath.c_str());

    return true;
  }

  entry = zip.FirstEntry();

  bool fileExistsInRestorePath = false;
  bool shouldContinue = false;
  
  // Iterating over each file in the zip and restoring the file if necessary
  do {
    std::string filePath = restorePath + "/" + entry->fileName();
    fileExistsInRestorePath = fileAccess.FileExists(filePath.c_str());

    if(!fileExistsInRestorePath) {
      // If the file has been deleted in source, it is restored.
      zip.UnzipMatching(restorePath.c_str(), entry->fileName(), false);
      entry = entry->NextEntry();
      continue;
    }

    long entryLastModifiedTs = entry->GetDt()->GetAsUnixTime(true);
    long fileLastModifiedTs = fileAccess.GetLastModified(filePath.c_str())->GetAsUnixTime(true);
    
    if(entryLastModifiedTs != fileLastModifiedTs) {
      // If the file has been modified in source, it is replaced.
      zip.UnzipMatching(restorePath.c_str(), entry->fileName(), false);
      entry = entry->NextEntry();
      continue;
    }

    nextEntry = entry->NextEntry();
    shouldContinue = entry->get_LastMethodSuccess();

    delete entry;
    entry = nextEntry;
  } while (shouldContinue);
  
  delete entry;
  zip.CloseZip();

  return true;
}

int main() {
  return 0;
}