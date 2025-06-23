const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Configurazione Dispositivo</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      padding: 0;
      background-color: #f4f4f9;
    }
    h1 {
      color: #333;
      margin-top: 20px;
    }
    p {
      color: #555;
      margin: 10px 0;
    }
    form {
      margin-top: 20px;
      background: #fff;
      padding: 20px;
      border-radius: 8px;
      box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
    }
    input[type="text"], input[type="password"], select, button {
      padding: 10px;
      font-size: 16px;
      margin: 10px 0;
      border: 1px solid #ccc;
      border-radius: 4px;
      width: 80%;
      max-width: 300px;
    }
    button {
      background-color: #28a745;
      color: white;
      border: none;
      cursor: pointer;
      transition: background-color 0.3s;
    }
    button:hover {
      background-color: #218838;
    }
    .device-select {
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <h1>Configurazione Dispositivo</h1>
  <p>Inserisci le credenziali Wi-Fi e seleziona il dispositivo da monitorare:</p>
  <form action="/save-device" method="GET">
    <!-- Campo SSID -->
    <input type="text" name="ssid" placeholder="Inserisci SSID della rete Wi-Fi" required>
    <br>
    <!-- Campo Password -->
    <input type="password" name="password" placeholder="Inserisci Password Wi-Fi" required>
    <br>
    <!-- Selezione dispositivo -->
    <div class="device-select">
      <select name="device" required>
        <option value="" disabled selected>-- Seleziona un dispositivo --</option>
        <option value="lavatrice">Lavatrice</option>
        <option value="frigorifero">Frigorifero</option>
        <option value="lavastoviglie">Lavastoviglie</option>
        <option value="asciugatrice">Asciugatrice</option>
        <option value="condizionatore">Condizionatore</option>
        <option value="forno">Forno</option>
        <option value="microonde">Microonde</option>
      </select>
    </div>
    <br>
    <button type="submit">Salva</button>
  </form>
</body>
</html>
)rawliteral";


const char CONFIRM_PAGE[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Device Selection Confirmation</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f9;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }
        .confirmation-container {
            text-align: center;
            background: #ffffff;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }
        .confirmation-container h1 {
            color: #4caf50;
            margin-bottom: 10px;
        }
        .confirmation-container p {
            margin: 10px 0;
        }
        .confirmation-container a {
            display: inline-block;
            margin-top: 15px;
            padding: 10px 20px;
            color: #ffffff;
            background-color: #4caf50;
            text-decoration: none;
            border-radius: 5px;
            font-weight: bold;
        }
        .confirmation-container a:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <div class="confirmation-container">
        <h1>Scelta Confermata!</h1>
        <p>Hai selezionato correttamente il dispositivo</p>
        <p>Grazie per la tua scelta</p>
    </div>
</body>
</html>
)rawliteral";