
#!/bin/python
# sensor-client.py
# Fermlog MQTT Sensor Client
# Handles incoming sensor data from FermLog system and logs to SQL


import paho.mqtt.client as mqtt
import mysql.connector
import json


broker_address="10.0.0.24"
topic_string = "fermlog/+/sensors/+"


# Callback for incoming mqtt sensor packets
def on_message(client, userdata, message):

    # Get system and sensor from mqtt topic
    topic_path = message.topic.split("/")
    system_name = topic_path[1]
    sensor_name = topic_path[3]

    print("FermLog sensor message received for '" + system_name + "'")
    print("Processing data for '" + sensor_name + "'")

    # Grab json payload
    payload = json.loads(str(message.payload.decode("utf-8")))

    # Connect to SQL
    print("Connecting to SQL")
    cnx = mysql.connector.connect(user='debian', password='temppwd', database='BeerDB')
    cursor = cnx.cursor()

    # Check if system exists
    cursor.execute("SELECT pk FROM system WHERE name = \"" + system_name + "\"")
    system_pk = 0
    num_rows = 0
    for (pk,) in cursor:
        system_pk = pk
        num_rows += 1
    if (num_rows is not 1 or system_pk < 1):
        print("System not found. Exiting.")
        return

    # Check if sensor exists
    cursor.execute("SELECT pk FROM sensor WHERE name = \"" + sensor_name + "\"")
    num_rows = 0
    for (pk,) in cursor:
        sensor_pk = pk
        num_rows += 1
    if (num_rows is not 1 or sensor_pk < 1):
        print("Sensor not found. Exiting.")
        return

    # Verified system and sensor. Add sensor value to sensor reading table
    print("Adding value to sensor table...")
    add_sensor_reading = ("INSERT INTO sensor_reading "
                          "(sensor_fk, t, value) "
                          "VALUES (%(sensor_fk)s, %(reading_time)s, %(reading_value)s)")
    data_sensor_reading = {
        'sensor_fk': sensor_pk,
        'reading_time': payload["t"],
        'reading_value': payload["value"]
    }
    try:
        cursor.execute(add_sensor_reading, data_sensor_reading)
        cnx.commit()
        print("Done.")
    except:
        print("Failed to add value to sensor table.")

    # Close SQL connection
    cursor.close()
    cnx.close()

# MAIN
sensor_client = mqtt.Client("FermLogSensorClient")
sensor_client.connect(broker_address)
sensor_client.subscribe(topic_string)
sensor_client.on_message = on_message   # subscription message callback
sensor_client.loop_start()              # start subscription thread

while(1):
    pass

print("program end")
