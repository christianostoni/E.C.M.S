from mysql.connector import  connect, Error


class mysqlConnect:
    def __init__(self):
        host = "mysql-2ba52429-scuola.d.aivencloud.com"
        user = "avnadmin"
        port = 18075
        password = "AVNS_RX806IjWRMWo9uSR8u0"
        database = "ECMS_demo_db"

        try:
            self.connection = connect(host=host, user=user, password=password, database=database, port=port)
            if self.connection.is_connected():
                print("mysql_connect.py: Connected to MySQL database")
        except Error as e:
            print(f"mysql_connect.py:  Error connecting to MySQL database: {e}")
        
    
    def insertData(self, timestamp, macAddress, data):
        try: 
            with self.connection.cursor() as cursor:
                #devo convertire il timestamp da unix in datetime(fomato UTC)
                query = f"INSERT INTO energy_consumption (updated_at, mac_address, sensorData) VALUES (FROM_UNIXTIME({timestamp}), '{macAddress}', {data})"
                cursor.execute(query)
                self.connection.commit()
        except Error as e:
            print(f"mysql_connect.py:   Error inserting data into MySQL database: {e}")
