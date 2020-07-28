// This is a modified version of the dashboard example from here:
// https://getbootstrap.com/docs/4.4/examples/dashboard/

const char DASHBOARD_TEMPLATE[] PROGMEM = { R"=====(<!doctype html>
<html lang='en'>
  <head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'>
    <meta name='theme-color' content='#563d7c'>
    <title>{-{TITLE}-}</title>
    <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css' integrity='sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh' crossorigin='anonymous'>
    <link rel='stylesheet' href='https://drewgreenwell.github.io/Arduino-WebHelper/html-templates/css/dashboard.css' />
  </head>
  <body>
    <nav class='navbar navbar-dark fixed-top bg-dark flex-md-nowrap p-0 shadow'>
      <a class='navbar-brand col-sm-3 col-md-2 mr-0' href='#'>Web Helper</a>
      <input class='form-control form-control-dark w-100' type='text' placeholder='Command' aria-label='Command'>
      <ul class='navbar-nav px-3'>
        <li class='nav-item text-nowrap'>
          <a class='nav-link' href='#' title='Reset' data-toggle="tooltip"><span data-feather='refresh-cw'></span></a>
          <a class='nav-link' href='#' title='Power Off' data-toggle="tooltip"><span data-feather='power'></span></a>
        </li>
      </ul>
    </nav>
      <div class='container-fluid'>
        <div class='row'>
          <nav class='col-md-2 d-none d-md-block bg-light sidebar'>
            <div class='sidebar-sticky'>
              <ul class='nav flex-column'>
                <li class='nav-item'>
                  <a class='nav-link active' href='/dashboard'>
                    <span data-feather='home'></span>
                    Dashboard <span class='sr-only'>(current)</span>
                  </a>
                </li>
                <li class='nav-item'>
                  <a class='nav-link' href='/reports'>
                    <span data-feather='bar-chart-2'></span>
                    Reports
                  </a>
                </li>
              </ul>
            </div>
          </nav>
      
          <main role='main' class='col-md-9 ml-sm-auto col-lg-10 px-4'>
            <div class='d-flex justify-content-between flex-wrap flex-md-nowrap align-items-center pt-3 pb-2 mb-3 border-bottom'>
              <h1 class='h2'>Dashboard</h1>
              <div class='btn-toolbar mb-2 mb-md-0'>
                <div class='btn-group mr-2'>
                  <button type='button' class='btn btn-sm btn-outline-secondary'>Share</button>
                  <button type='button' class='btn btn-sm btn-outline-secondary'>Export</button>
                </div>
                <button type='button' class='btn btn-sm btn-outline-secondary dropdown-toggle'>
                  <span data-feather='calendar'></span>
                  This week
                </button>
              </div>
            </div>      
            <canvas class='my-4 w-100' id='myChart' width='900' height='380'></canvas>
            {-{BODY}-}
            <h2>Section title</h2>
            <div class='table-responsive'>
              <table class='table table-striped table-sm'>
                <thead>
                  <tr>
                    <th>#</th>
                    <th>Header</th>
                    <th>Header</th>
                    <th>Header</th>
                    <th>Header</th>
                  </tr>
                </thead>
                <tbody>
                  <tr>
                    <td>1,001</td>
                    <td>Lorem</td>
                    <td>ipsum</td>
                    <td>dolor</td>
                    <td>sit</td>
                  </tr>
                  <tr>
                    <td>1,002</td>
                    <td>amet</td>
                    <td>consectetur</td>
                    <td>adipiscing</td>
                    <td>elit</td>
                  </tr>
                  <tr>
                    <td>1,003</td>
                    <td>Integer</td>
                    <td>nec</td>
                    <td>odio</td>
                    <td>Praesent</td>
                  </tr>
                </tbody>
              </table>
            </div>
          </main>
        </div>
      </div>
      <script src='https://code.jquery.com/jquery-3.4.1.slim.min.js' integrity='sha384-J6qa4849blE2+poT4WnyKhv5vZF5SrPo0iEjwBvKU7imGFAV0wwj1yYfoRSJoZ+n' crossorigin='anonymous'></script>
      <script src='https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js' integrity='sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo' crossorigin='anonymous'></script>
      <script src='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js' integrity='sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6' crossorigin='anonymous'></script>
      <script src='https://cdnjs.cloudflare.com/ajax/libs/feather-icons/4.9.0/feather.min.js'></script>
      <script src='https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.3/Chart.min.js'></script>
      <script src='https://drewgreenwell.github.io/Arduino-WebHelper/html-templates/js/dashboard.js'></script>
    </body>
</html>)====="};
