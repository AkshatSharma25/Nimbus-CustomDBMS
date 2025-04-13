import subprocess
import threading
import time

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

    # Send command
    commands = [
        "showTables"
    ]

    for cmd in commands:
        print(f"Sending:{cmd}")
        process.stdin.write(cmd + "\n")
        process.stdin.flush()
        time.sleep(1)

    # Let it run a bit to print output
    time.sleep(3)

    # Close input and wait for process to finish
    process.stdin.close()
    process.wait()
    print("Subprocess finished.")

if __name__ == "__main__":
    main()
