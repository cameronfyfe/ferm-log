# System modules
from datetime import datetime
import mysql.connector
from flask import (
    make_response,
    abort
)

def get_all(system_name):

    print("--> get_all(" + system_name + ")")

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

    # Get sensors for system
    sensor_pks = []
    cursor.execute("SELECT pk FROM sensor WHERE system_fk = " + system_pk)
    num_rows = 0
    for (pk,) in cursor:
        sensor_pks.add(pk)
        num_rows += 1
    if (num_rows < 1):
        print("No sensors found. Exiting.")
        return

    sensors = []
    for sensor_pk in sensor_pks:
        sensor_data = []
        cursor.execute("SELECT * FROM sensor_reading WHERE sensor_pk = " + sensor_pk)
        for (value,) in cursor:
            sensor_data.add(value)
        sensors.add(sensor_data)

    return sensors
