package backapp.bean;

import java.time.LocalDateTime;

public class Backup {
  public int id;
  public String source;
  public String destination;
  public LocalDateTime timestamp;

  public Backup(
    int id,
    String source,
    String destination,
    LocalDateTime timestamp
  ) {
    this.id = id;
    this.source = source;
    this.destination = destination;
    this.timestamp = timestamp;
  }

  public Backup(
    String source,
    String destination,
    LocalDateTime timestamp
  ) {
    this.source = source;
    this.destination = destination;
    this.timestamp = timestamp;
  }

  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

  public String getSource() {
    return source;
  }

  public void setSource(String source) {
    this.source = source;
  }

  public String getDestination() {
    return destination;
  }

  public void setDestination(String destination) {
    this.destination = destination;
  }

  public LocalDateTime getTimestamp() {
    return timestamp;
  }

  public void setTimestamp(LocalDateTime timestamp) {
    this.timestamp = timestamp;
  }
}