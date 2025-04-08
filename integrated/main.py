import subprocess
import threading
import time
import csv

CSV_FILE = "data.csv"  # Make sure this file exists with 10,000 records

# Start the nimbus process
process = subprocess.Popen(
    "./nimbus", stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, bufsize=1
)

# Function to continuously read output
def read_output():
    for line in iter(process.stdout.readline, ''):
        print(line.strip())

# Run output reader in a separate thread
thread = threading.Thread(target=read_output, daemon=True)
thread.start()

# Send initialization commands
init_commands = [
    "createDatabase.test5",
    "useDatabase.test5",
    "db.createTable.(name:mytable,stName:char[50],age:int,gender:char)"
]

for command in init_commands:
    print(f"Sending: {command}")
    process.stdin.write(command + "\n")
    process.stdin.flush()
    time.sleep(1)

num=0
# Read CSV and insert records
with open(CSV_FILE, "r") as file:
    reader = csv.reader(file)
    next(reader)  # Skip header
    for row in reader:
        st_name, age, gender = row
        insert_command = f"db.insert.(name:mytable,{st_name},{age},{gender})"
        print(f"Sending: {insert_command}")
        process.stdin.write(insert_command + "\n")
        process.stdin.flush()
        time.sleep(0.1) 

# Close process
process.stdin.close()
process.wait()
