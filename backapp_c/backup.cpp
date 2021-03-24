#include <iostream>
#include <chrono>

#include "include/backapp_nativelink_Backup.h"
#include "include/CkZip.h"
#include "include/CkDirTree.h"
#include "include/CkFileAccess.h"
#include "include/CkZipEntry.h"
#include "include/CkDateTime.h"
#include "include/CkBinData.h"
#include "include/CkCompression.h"
#include "include/CkGzip.h"

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
  CkDirTree dirTree;
  CkBinData bdata;
  // CkCompression compress;
  CkGzip gzip;
  CkZipEntry *entry;
  CkDateTime dateTime;
  
  const char *convertedValue;
  const int AES_CODE = 4;
	const int AES_ENCRYPTION_LENGTH = 128;
  const int MAX_READ_BYTES = 99999999;
  bool success = false;
  // const std::string COMPRESSION_ALGORITHM = "deflate";
  // compress.put_Algorithm(COMPRESSION_ALGORITHM.c_str());

  gzip.put_CompressionLevel(6);

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
  
  dirTree.put_BaseDir(source.c_str());
  dirTree.put_Recurse(true);
  dirTree.BeginIterate();

  while (!dirTree.get_DoneIterating()) {
    // Iterate the files in the source directory,
    // compress them and add them to zip

    if(dirTree.get_IsDirectory()) {
      // Skip if directory
      success = dirTree.AdvancePosition();
      if (success != true) {
        if (dirTree.get_DoneIterating() != true) {
          std::cout << dirTree.lastErrorText() << "\r\n";
          return false;
        }
      }
    }

    fileAccess.OpenForRead(dirTree.fullPath());

    bdata.Clear();
    fileAccess.FileReadBd(MAX_READ_BYTES, bdata);

    gzip.CompressBd(bdata);

    zip.AppendBd(dirTree.relativePath(), bdata);

    // When compressed, the files last modified date changes.
    // This is to reset the last modified date back to original.
    entry = zip.FirstMatchingEntry(dirTree.relativePath());
    entry->SetDt(*(fileAccess.GetLastModified(dirTree.fullPath())));

    delete entry;

    success = dirTree.AdvancePosition();
    if (success != true) {
      if (dirTree.get_DoneIterating() != true) {
        std::cout << dirTree.lastErrorText() << "\r\n";
        return false;
      }
    }
  }

  // zip.AppendFiles((source + "/*").c_str(), true);

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
  CkBinData bdata;
  CkGzip gzip;
  const char *convertedValue;
  // CkCompression compress;
  // const std::string COMPRESSION_ALGORITHM = "deflate";
  // compress.put_Algorithm(COMPRESSION_ALGORITHM.c_str());
  
  gzip.put_CompressionLevel(6);

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
    fileAccess.DirEnsureExists(restorePath.c_str());
  }

  entry = zip.FirstEntry();

  bool fileExistsInRestorePath = false;
  bool shouldContinue = false;
  long entryLastModifiedTs;
  long fileLastModifiedTs;

  // Iterating over each file in the zip and restoring the file if necessary
  do {
    if(entry->get_IsDirectory()) {

      nextEntry = entry->NextEntry();
      shouldContinue = entry->get_LastMethodSuccess();

      delete entry;
      entry = nextEntry;
      continue;
    }
    std::string filePath = restorePath + "/" + entry->fileName();

    fileExistsInRestorePath = fileAccess.FileExists(filePath.c_str());

    if(!fileExistsInRestorePath) {
      // If the file has been deleted in source, it is restored.
      // zip.UnzipMatching(restorePath.c_str(), entry->fileName(), false);
      bdata.Clear();
      entry->UnzipToBd(bdata);

      gzip.UncompressBd(bdata);
    
      fileAccess.OpenForWrite(filePath.c_str());
      fileAccess.FileWriteBd(bdata, 0, 0);
      fileAccess.FileClose();


    } else {
      // If the file has been modified in source, it is replaced.
      // zip.UnzipMatching(restorePath.c_str(), entry->fileName(), false);
      entryLastModifiedTs = entry->GetDt()->GetAsUnixTime(true);
      fileLastModifiedTs = fileAccess.GetLastModified(filePath.c_str())->GetAsUnixTime(true);

      if (entryLastModifiedTs != fileLastModifiedTs) {
        bdata.Clear();
        entry->UnzipToBd(bdata);

        gzip.UncompressBd(bdata);
        
        fileAccess.OpenForWrite(filePath.c_str());
        fileAccess.FileWriteBd(bdata, 0, 0);
        fileAccess.FileClose();
      }

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