import serial
import csv
import os
import datetime
import logging

logging.basicConfig(level=logging.ERROR, format='%(asctime)s - %(levelname)s - %(message)s')

class MessageReader:
    def __init__(self, serial_port_number=3):
        self.serial_port_number = serial_port_number
        self.serial_port = self.start_serial()

    def start_serial(self):
        return serial.Serial(
            port=f"COM{self.serial_port_number}",
            baudrate=115200,
            bytesize=8,
            timeout=2,
            stopbits=serial.STOPBITS_ONE,
        )

    def read_message(self):
        """Reads data from the serial port, processes it, and returns a complete message."""
        bufferstring = ""
        while self.serial_port.in_waiting > 0:
            serial_string = self.serial_port.read()
            newchar = serial_string.decode("ascii", errors="ignore")
            bufferstring += newchar
            if newchar == "\n":
                bufferstring = bufferstring.replace("\r", "").strip()
                if not (len(bufferstring) == 1 or "FWT" in bufferstring or "!FWADR" in bufferstring):
                    return bufferstring
        return None


class FeatherweightParsing:
    def __init__(self):
        self.packet_headers = []
        self.header_files = []
        self.message_reader = MessageReader()  # Use the new MessageReader class

    def main(self):
        # starting_date_string, starting_time_string = self.start_datetime()
        # self.navigate_datetime_directory(starting_date_string, starting_time_string)
        while True:
            message = self.message_reader.read_message()
            if message:
                self.parse_message(message)

    def start_datetime(self):
        starting_datetime = datetime.datetime.now()
        starting_date_string = starting_datetime.strftime("%Y-%m-%d")
        starting_time_string = starting_datetime.strftime("%H%M%S")
        return starting_date_string, starting_time_string

    def navigate_datetime_directory(self, starting_date_string, starting_time_string):
        if not os.path.isdir("logs"):
            os.mkdir("logs")
        os.chdir("logs")
        if not os.path.isdir(starting_date_string):
            os.mkdir(starting_date_string)
        os.chdir(starting_date_string)
        os.mkdir(starting_time_string)
        os.chdir(starting_time_string)

    def split_message(self, message: str) -> list:
        message_split_list = message.split(" ")
        message_split_list = [split for split in message_split_list if len(split) > 0]
        message_split_list = message_split_list[:-1]
        return message_split_list

    def parse_message(self, message: str):
        message_split = self.split_message(message)
        if len(message_split) < 1 or not message_split[0] == "@":
            return
        header = message_split[1]
        if header not in self.packet_headers:
            self.packet_headers.append(header)
            with open(f"{header}.csv", "w") as f:
                pass

        with open(f"{header}.csv", "a") as f: # if we want pretty, newline = "
            writer = csv.writer(f, delimiter=",")
            writer.writerow(message_split)
            if message_split[1] == "GPS_STAT":
                self.parse_gps_to_csv(message)
                self.print_gps_terminal(message)

########################################################################################

    def print_gps_terminal(self, message: str):
        message_split = self.split_message(message)
        if len(message_split) < 1 or not message_split[0] == "@":
            return
        if message_split[1] == "GPS_STAT":
            print(
                message_split[8],
                message_split[9],
                message_split[11],
                message_split[13],
                message_split[15],
                message_split[17],
                message_split[19],
            )

    def extract_gps_coords(self, message: str):
        message_split = self.split_message(message)
        if len(message_split) < 1 or not message_split[0] == "@":
            return (None, None, None, None, None)
        if message_split[1] == "GPS_STAT":
            return (
                float(message_split[11]),
                float(message_split[13]),
                float(message_split[15]),
                float(message_split[17]),
                float(message_split[19]),
            )
        return (None, None, None, None, None)

########################################################################################

    def parse_gps_to_csv(self, message: str):

        print("CSV CALLED (FOR ERROR CHECKING)")

        message_split = self.split_message(message)

        if len(message_split) < 1 or message_split[0] != "@":
            return


        if message_split[1] != "GPS_STAT":
            return


        file_path = "VISUALIZER_DATA.csv"
        file_exists = os.path.isfile(file_path)


        if not file_exists:
            try:
                with open(file_path, "w", newline="") as f:
                    writer = csv.writer(f, delimiter=",")
                    writer.writerow(["Altitude (ft)", "Latitude (ft)", "Longitude (ft)", "Date (YYYY-MM-DD)", "Time"])
            except Exception as e:
                logging.error(f"Error creating CSV file: {e}")
                return


        try:
            altitude = message_split[11]
            latitude = message_split[13]
            longitude = message_split[15]
            time = message_split[6]
            loggingdatetime = datetime.datetime.now()
            date = loggingdatetime.strftime("%Y-%m-%d")


            with open(file_path, "a", newline="") as f:
                writer = csv.writer(f, delimiter=",")
                writer.writerow([altitude, latitude, longitude, date, time])

        except Exception as e:
            logging.error(f"Error parsing GPS to CSV: {e}")

########################################################################################

if __name__ == "__main__":
    interface = FeatherweightParsing()
    interface.main()