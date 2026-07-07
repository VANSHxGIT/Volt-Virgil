#ifndef DASHBOARD_H
#define DASHBOARD_H

const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>VoltVigil Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial; text-align: center; background-color: #121212; color: #ffffff; margin-top: 50px;}
    h1 { color: #00ADB5; }
    .metric { font-size: 3rem; font-weight: bold; margin: 20px 0; }
    .excellent { color: #00FF00; }
    .tripped { color: #FF0000; }
  </style>
</head>
<body>
  <h1>VoltVigil Status</h1>
  <div class="metric" id="volts">-- V</div>
  <h2 id="status">CONNECTING...</h2>

  <script>
    setInterval(function ( ) {
      fetch('/readings')
        .then(response => response.json())
        .then(data => {
          let v = parseFloat(data.v);
          document.getElementById('volts').innerText = v.toFixed(1) + " V";
          
          let statusEl = document.getElementById('status');
          if (v >= %MIN_VOLTS% && v <= %MAX_VOLTS%) {
            statusEl.innerText = "SYSTEM: EXCELLENT";
            statusEl.className = "excellent";
          } else {
            statusEl.innerText = "SYSTEM: TRIPPED";
            statusEl.className = "tripped";
          }
        });
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

#endif