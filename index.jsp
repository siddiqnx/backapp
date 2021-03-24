<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>

<c:set var="path" scope="session" value="${pageContext.request.contextPath}"/>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>BackApp</title>

    <link href="${path}/css/materialize.min.css" rel="stylesheet" type="text/css" />
    <link href="${path}/css/style.css" rel="stylesheet" type="text/css" />
  </head>
  <body>

    <div class="layout">
      <header id="header" class="row valign-wrapper">
        <h3 class="col s8 page-title valign">BackApp</h3>
      </header>
      <div class="row">

          <!-- Sidebar -->
          <aside id="sidebar" class="col s3">
            <div class="row">
              <header class="col s12">
                <h5>Create a Backup</h5>
              </header>
            </div>
            <div class="row">

              <!-- Create Backup Form -->
              <form id="create-backup" class="col s10" action="${path}/backups/create" method="POST">
                
                <div class="row">
                  <div class="input-field col s12">
                    <input name="source" id="source" type="text" class="validate">
                    <label for="source">Source</label>
                  </div>
                </div>
                
                <div class="row">
                  <div class="input-field col s12">
                    <textarea name="destination" id="destination" class="materialize-textarea"></textarea>
                    <label for="destination">Destination</label>
                  </div>
                </div>
        
                <div class="row">
                  <div class="input-field col s12">
                    <input name="ekey" id="ekey" type="text" class="validate">
                    <label for="ekey">Password</label>
                  </div>
                </div>
        
                <button type="submit" class="waves-effect waves-light btn">Create Backup</button>
              </form>
            </div>
          </aside>

        <!-- Main Content -->
        <main id="backups-table-container" class="col s9">
        
          <div class="row">

            <table id="backups-table" class="col s9 responsive-table">
              <thead>
                <tr>
                    <th>Source</th>
                    <th>Destination</th>
                    <th>Last Backup</th>
                    <th></th>
                </tr>
              </thead>

              <tbody>
                <c:forEach items="${backups}" var="backup">
                  <tr>
                    <td class="hide id">${backup.id}</td>
                    <td>${backup.source}</td>
                    <td>${backup.destination}</td>
                    <td>${backup.timestamp.toString()}</td>
                    <td><a class="restore-btn waves-effect waves-teal btn-flat grey lighten-5" href="#">Restore</a></td>
                    <td>
                      <a href="#">
                        <img
                          class="delete-icon"
                          src="${path}/images/delete_icon.svg"
                        >
                      </a>
                    </td>
                  </tr>
                </c:forEach>
              </tbody>
            </table>

          </div>

        </main>
      </div>
    </div>
    
    <div id="restore-modal" class="modal">
      <div class="row">
        <div class=" col s8 modal-content">
          <h5 class="modal-title">Enter password</h5>
          <form id="password-form" action="" method="GET">
            <div class="valign-wrapper">
              <div class="hide valign input-field col s2">
                <input value="" name="id" id="id" type="text" class="validate">
                <label for="id">Id</label>
              </div>
              <div class="valign input-field col s5">
                <input value="" name="password" id="password" type="password" class="validate">
                <label for="password">Password</label>
              </div>
              <div class="valign input-field col s1">
                <button type="submit" class="password-submit-btn waves-effect waves-light btn"></button>
              </div>
            </div>
          </form>
        </div>
      </div>
    </div>

    <script src="${path}/js/materialize.min.js"></script>
    <script defer>
      const backupsTable = document.querySelector('#backups-table');
      const restoreModal = document.querySelector('#restore-modal');
      
      M.Modal.init(restoreModal);
      const modal = M.Modal.getInstance(restoreModal);
      
      backupsTable.addEventListener('click', (e) => {
        e.preventDefault();
        if(e.target.classList.contains("restore-btn")) {
          const target = e.target.closest('tr');
          const backupId = target.querySelector('.id').textContent;
          
          const form = restoreModal.querySelector('#password-form');
          const idInputEl = restoreModal.querySelector('.input-field > #id');
          const passwordSubmitBtn = restoreModal.querySelector('.password-submit-btn');
          
          form.setAttribute('action', '${path}/backups/restore');
          idInputEl.setAttribute("value", backupId);
          passwordSubmitBtn.textContent = 'Restore';

          modal.open();
        } else if(e.target.classList.contains("delete-icon")) {
          
          const target = e.target.closest('tr');
          const backupId = target.querySelector('.id').textContent;
          
          const form = restoreModal.querySelector('#password-form');
          const idInputEl = restoreModal.querySelector('.input-field > #id');
          const passwordSubmitBtn = restoreModal.querySelector('.password-submit-btn');
          
          form.setAttribute('action', '${path}/backups/delete');
          idInputEl.setAttribute("value", backupId);
          passwordSubmitBtn.textContent = 'Delete';
          modal.open();
        }
      });
    </script>

  </body>
</html>
