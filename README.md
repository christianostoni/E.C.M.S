# ⚡ Energy Consumption Monitoring System (E.C.M.S.)

## 🔍 Overview

**E.C.M.S.** è un sistema progettato per monitorare in tempo reale il consumo energetico degli elettrodomestici domestici. Nasce da un’esperienza di tirocinio presso Minervas e viene poi esteso come progetto scolastico per un percorso di informatica industriale.

Attraverso l’uso di microcontrollori ESP8266, sensori di corrente ACS712 e una dashboard in Grafana, ECMS consente di:
- Visualizzare in tempo reale l’energia assorbita dai dispositivi.
- Analizzare serie storiche dei consumi.
- Ricevere notifiche in caso di picchi di potenza.
- Avere una visione oggettiva e consapevole dell’utilizzo energetico domestico.

## 🛠️ Architettura del Sistema

- **ESP8266**: rileva la corrente tramite ACS712 e invia i dati via UDP al Raspberry Pi.
- **Raspberry Pi**:
  - Riceve dati via socket UDP.
  - Pubblica i dati su un **broker MQTT remoto (EMQX)**.
  - Inserisce i dati in un **database MySQL**.
  - Visualizza i dati in tempo reale tramite **Grafana**.
- **Grafana**: dashboard con:
  - Grafici temporali in Ampere e Watt.
  - Gauge per consumo cumulativo.
  - Media giornaliera in Wh.
  - Alert configurabili.
- **PCB e stampa 3D**: box elettrico progettato con Fusion 360, realizzato in PLA, con circuiteria custom.

## 📦 Struttura del Repository

```
ecms/
├── arduino/              # Codice ESP8266
│   └── ecms_device.ino
├── src/                  # Codice server Python su Raspberry Pi
│   ├── main.py
│   ├── mqtt_to_mysql.py
│   └── mqtt_publisher.py
└── README.md
```

## 🚀 Avvio rapido

### Prerequisiti

- Raspberry Pi con Python 3 e Grafana installati.
- Broker MQTT remoto (es. EMQX).
- Database MySQL accessibile da remoto.
- Rete Wi-Fi domestica stabile.

### Installazione

1. Carica il firmware su ogni ESP8266 via Arduino IDE.
2. Configura il Wi-Fi tramite interfaccia web dell’ESP (SSID: `ECMS_device`).
3. Avvia i due processi Python come `systemd` service:
   ```bash
   sudo systemctl start ecms_udp_listener
   sudo systemctl start ecms_mqtt_mysql
   ```
4. Importa la dashboard JSON su Grafana.
5. Accedi a Grafana (`http://<raspberry_ip>:3000`) per visualizzare i dati.

## 📊 Funzionalità principali

- 📈 Serie storiche in Ampere e Watt
- ⚠️ Alert preconfigurati (es. soglia 2.5kW)
- 🔧 Supporto multi-utente (prototipale)
- 🔒 Sicurezza TLS su connessione MQTT
- 🔌 PCB personalizzata con case stampato in 3D

## 🧠 Idea chiave

> "Non possiamo migliorare ciò che non possiamo misurare."

ECMS nasce con l’obiettivo di **rendere visibile e comprensibile ciò che normalmente sfugge**: il consumo energetico. Attraverso una piattaforma accessibile e modulare, l’utente è guidato verso scelte più consapevoli.

## 👨‍💻 Autore

**Christian Giuseppe Ostoni**  
Studene IIS Della-Corte Vanvitelli
📍 Cava De' Tirreni, Italia  
🚀 Progetto di fine ciclo scolastico – 2024/2025

## 📄 Licenza

Questo progetto è rilasciato sotto licenza **MIT**. Puoi modificarlo, usarlo e redistribuirlo liberamente.
