import subprocess
import threading
import time

# Use stdbuf to prevent buffering issues
process = subprocess.Popen(
    ["stdbuf", "-oL", "../nimbus"],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True,
    bufsize=1
)

def read_output():
    for line in iter(process.stdout.readline, ''):
        if line:
            print("STDOUT:", line.strip())

def read_stderr():
    for line in iter(process.stderr.readline, ''):
        if line:
            print("STDERR:", line.strip())

threading.Thread(target=read_output, daemon=True).start()
threading.Thread(target=read_stderr, daemon=True).start()

init_commands = [
    # "showTables",
    # "db.find.(name:stx,{})"
]

for command in init_commands:
    print(f"Sending: {command}")
    process.stdin.write(command + "\n")
    process.stdin.flush()
    time.sleep(1)

# Wait a bit to receive output before killing the process
time.sleep(5)
process.stdin.close()
process.wait()
