import subprocess
import threading
import time
import csv

def read_stdout(process):
    for line in iter(process.stdout.readline, ''):
        if line:
            print("STDOUT:", line.strip())

def read_stderr(process):
    for line in iter(process.stderr.readline, ''):
        if line:
            print("STDERR:", line.strip())

def main():
    # Start subprocess
    process = subprocess.Popen(
        ["./nimbus"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1
    )

    # Start threads to read stdout and stderr
    threading.Thread(target=read_stdout, args=(process,), daemon=True).start()
    threading.Thread(target=read_stderr, args=(process,), daemon=True).start()

    # Initialize database and table
    init_commands = [
        "createDatabase.csvdata",
        "useDatabase.csvdata",
        "db.createTable.(name:people,stName:char[20],age:int,something:char[20])"
    ]

    # Read data from CSV
    insert_commands = []
    with open('data.csv', 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        next(csvreader)  # Skip header row
        for row in csvreader:
            # Format: stName, age, something
            if len(row) == 3:  # Ensure we have all columns
                stName, age, something = row
                insert_cmd = f"db.insert.(name:people,{stName},{int(age)},{something})"
                insert_commands.append(insert_cmd)

    # Verification commands
    verify_commands = [
        "showTables",
        "db.find.(name:people,{})",  # Find all records
        f"db.find.(name:people,{{age>50}})",  # Find people older than 50
        f"db.find.(name:people,{{something==xyz}})"  # Find records where something=xyz
    ]

    # Combine all commands
    commands = init_commands + insert_commands + verify_commands

    # Send commands to the database
    for cmd in commands:
        print(f"Sending: {cmd}")
        process.stdin.write(cmd + "\n")
        process.stdin.flush()
        time.sleep(0.1)  # Reduced sleep time for faster processing

    # Let it run a bit to print output
    time.sleep(3)

    # Close input and wait for process to finish
    process.stdin.close()
    process.wait()
    print("Subprocess finished.")

if __name__ == "__main__":
    main()