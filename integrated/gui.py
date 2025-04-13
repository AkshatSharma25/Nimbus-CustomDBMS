import tkinter as tk
from tkinter import ttk
import subprocess
import threading
import queue
import os
import math
import time


class LoginScreen:
    def __init__(self, root):
        self.root = root
        self.root.title("Nimbus - Login")
        self.root.geometry("400x300")
        self.root.resizable(False, False)

        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        x = (screen_width - 400) // 2
        y = (screen_height - 300) // 2
        self.root.geometry(f"400x300+{x}+{y}")

        style = ttk.Style()
        style.configure("TLabel", font=("Segoe UI", 10))
        style.configure("TEntry", padding=5)
        style.configure("TButton", padding=5, font=("Segoe UI", 10, "bold"))

        self.main_frame = ttk.Frame(root)
        self.main_frame.pack(fill="both", expand=True, padx=20, pady=20)

        ttk.Label(self.main_frame, text="Nimbus", font=("Segoe UI", 24, "bold")).pack(pady=10)

        form_frame = ttk.Frame(self.main_frame)
        form_frame.pack(pady=20)

        ttk.Label(form_frame, text="Username:").grid(row=0, column=0, sticky="w", pady=5, padx=(0, 10))
        self.username_entry = ttk.Entry(form_frame, width=30)
        self.username_entry.grid(row=0, column=1, pady=5)

        ttk.Label(form_frame, text="Password:").grid(row=1, column=0, sticky="w", pady=5, padx=(0, 10))
        self.password_entry = ttk.Entry(form_frame, show="*", width=30)
        self.password_entry.grid(row=1, column=1, pady=5)

        ttk.Button(self.main_frame, text="Login", command=self.attempt_login).pack(pady=10)

        self.status_label = ttk.Label(self.main_frame, text="", foreground="red")
        self.status_label.pack()

    def attempt_login(self):
        username = self.username_entry.get()
        password = self.password_entry.get()

        if username == "admin" and password == "password":
            self.status_label.config(text="Login successful!", foreground="green")
            self.root.after(1000, self.open_main_app)
        else:
            self.status_label.config(text="Invalid credentials", foreground="red")

    def open_main_app(self):
        self.root.destroy()
        root = tk.Tk()
        app = DatabaseApp(root)
        root.mainloop()


class DatabaseApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Nimbus - Database Management System")
        self.root.geometry("1000x700")
        self.root.minsize(800, 600)

        self.information = []

        # Setup output queue and processing
        self.output_queue = queue.Queue()
        self.after_id = None
        
        # Start the nimbus process
        self.process = None
        self.start_nimbus_process()

        # Configure styles
        self.style = ttk.Style()
        self.style.theme_use("clam")
        self.style.configure("Sidebar.TFrame", background="#34495e")
        self.style.configure("Sidebar.TButton", background="#34495e", foreground="white", 
                            font=("Segoe UI", 10, "bold"))
        self.style.map("Sidebar.TButton", background=[("active", "#2c3e50")])
        
        # Text widget tags
        self.style.configure("Sent.TText", foreground="blue")
        self.style.configure("Stdout.TText", foreground="black")
        self.style.configure("Stderr.TText", foreground="red")
        self.style.configure("Error.TText", foreground="red", font=("Segoe UI", 9, "italic"))

        # Main container
        self.main_container = ttk.Frame(root)
        self.main_container.pack(fill="both", expand=True)

        # Sidebar
        self.sidebar = ttk.Frame(self.main_container, width=200, style="Sidebar.TFrame")
        self.sidebar.pack(side="left", fill="y")

        ttk.Label(self.sidebar, text="Nimbus", background="#34495e",
                 foreground="white", font=("Segoe UI", 16, "bold")).pack(pady=20, padx=10)

        self.active_tab = tk.StringVar(value="Dashboard")
        menu_items = ["Dashboard", "Documents", "Query"]
        for item in menu_items:
            btn = ttk.Button(self.sidebar, text=item, command=lambda i=item: self.show_content(i),
                            style="Sidebar.TButton")
            btn.pack(fill="x", padx=10, pady=2)

        # Content area
        self.content_area = ttk.Frame(self.main_container)
        self.content_area.pack(side="right", fill="both", expand=True, padx=10, pady=10)

        # Start queue processing
        self.process_queue()
        
        # Show initial content
        self.show_content("Dashboard")
        


    def process_queue(self):
        """Process items from the output queue in the main thread"""
        try:
            while True:
                item = self.output_queue.get_nowait()
                callback, args = item
                callback(*args)
        except queue.Empty:
            pass
        
        # Schedule the next queue processing
        self.after_id = self.root.after(100, self.process_queue)

    def start_nimbus_process(self):
        """Start the nimbus subprocess"""
        try:
            self.process = subprocess.Popen(
                ["./nimbus"],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1,
                universal_newlines=True
            )
            print(f"Process started with PID: {self.process.pid}")
        except Exception as e:
            print(f"❌ Failed to start process: {e}")
            self.process = None

    def restart_process(self):
        """Restart the nimbus process"""
        if self.process:
            try:
                self.process.terminate()
                self.process.wait(timeout=1)
            except:
                pass
        self.start_nimbus_process()

    def show_content(self, content_name):
        """Show the selected content in the main area"""
        for widget in self.content_area.winfo_children():
            widget.destroy()

        ttk.Label(self.content_area, text=content_name, font=("Segoe UI", 18, "bold")).pack(anchor="nw", pady=10)

        if content_name == "Dashboard":
            self.show_dashboard()
        elif content_name == "Documents":
            self.show_documents()
        elif content_name == "Query":
            self.show_query()

    def show_dashboard(self):
        """Show dashboard content"""
        stats_frame = ttk.Frame(self.content_area)
        stats_frame.pack(fill="x", pady=10)
        total_size = self.get_readable_folder_size()
        total_databases=self.count_folders("databases")
        total_folder=self.count_total_folders("databases")
        stats = [
            {"title": "Databases", "value": total_databases, "color": "#3498db"},
            {"title": "Tables", "value": total_folder-total_databases, "color": "#2ecc71"},
            {"title": "Storage", "value": total_size, "color": "#e67e22"}
        ]

        for stat in stats:
            frame = ttk.Frame(stats_frame, relief="ridge", padding=15)
            frame.pack(side="left", expand=True, fill="x", padx=5)

            ttk.Label(frame, text=stat["title"], font=("Segoe UI", 10, "bold")).pack()
            ttk.Label(frame, text=stat["value"], font=("Segoe UI", 20, "bold"),
                     foreground=stat["color"]).pack()


    def show_documents(self):
        """Show documents content"""
        ttk.Label(self.content_area, text="Document Explorer").pack(anchor="w")
        ttk.Label(self.content_area, text="Select a database to view documents").pack(anchor="w")

    def show_query(self):
        """Show query interface"""
        query_frame = ttk.Frame(self.content_area)
        query_frame.pack(fill="x", pady=10)

        ttk.Label(query_frame, text="Enter Query:").pack(side="left")
        self.query_entry = ttk.Entry(query_frame, width=60)
        self.query_entry.pack(side="left", padx=5)

        ttk.Button(query_frame, text="Execute",
                  command=lambda: self.execute_query(self.query_entry.get())).pack(side="left", padx=5)
        
        ttk.Button(query_frame, text="Clear",
                  command=self.clear_output).pack(side="left", padx=5)

        ttk.Separator(self.content_area, orient="horizontal").pack(fill="x", pady=10)

        ttk.Label(self.content_area, text="Query Results:").pack(anchor="nw")
        
        self.result_box = tk.Text(
            self.content_area, 
            height=20, 
            bg="#ffffff", 
            font=("Courier", 10),
            wrap=tk.WORD
        )
        self.result_box.pack(fill="both", expand=True, pady=5)
        self.result_box.config(state="disabled")
        
        # Configure text tags
        self.result_box.tag_config("sent", foreground="blue")
        self.result_box.tag_config("stdout", foreground="black")
        self.result_box.tag_config("stderr", foreground="red")
        self.result_box.tag_config("error", foreground="red", font=("Segoe UI", 9, "italic"))

    def clear_output(self):
        """Clear the output box"""
        self.result_box.config(state="normal")
        self.result_box.delete(1.0, tk.END)
        self.result_box.config(state="disabled")

    def execute_query(self, query):
        """Execute a query in the nimbus process"""
        if not query.strip():
            return

        try:
            if not self.process or self.process.poll() is not None:
                self.append_output("⚠ Nimbus process is not running. Restarting...\n", "error")
                self.restart_process()
                if not self.process or self.process.poll() is not None:
                    self.append_output("❌ Failed to restart Nimbus process\n", "error")
                    return

            # Clear previous content
            self.clear_output()
            self.append_output(f"▶ Executing: {query}\n", "sent")

            # Write query to process
            self.process.stdin.write(query + "\n")
            self.process.stdin.flush()

            # Start output reader thread
            threading.Thread(
                target=self._read_stream,
                args=(self.process.stdout, "stdout"),
                daemon=True
            ).start()

            # Start error reader thread
            threading.Thread(
                target=self._read_stream,
                args=(self.process.stderr, "stderr"),
                daemon=True
            ).start()

        except Exception as e:
            self.append_output(f"❌ Error: {e}\n", "error")

    def _read_stream(self, stream, tag):
        """Read from a stream and put output in queue"""
        try:
            line=stream.readline()
            while True:
                line = stream.readline()
                if not line:
                    break
                self.output_queue.put((
                    self.append_output,
                    (line, tag)
                ))
        except Exception as e:
            self.output_queue.put((
                self.append_output,
                (f"⚠ Stream read error: {e}\n", "error")
            ))

    def append_output(self, text, tag):
        """Thread-safe output appending"""
        self.result_box.config(state="normal")
        self.result_box.insert(tk.END, text, tag)
        self.result_box.see(tk.END)
        self.result_box.config(state="disabled")

    def __del__(self):
        """Clean up when closing"""
        if self.after_id:
            self.root.after_cancel(self.after_id)
        if self.process:
            try:
                self.process.terminate()
            except:
                pass

    def get_readable_folder_size(self):
        total_size = 0
        path="databases"
        allowed_extensions = ('.csv', '.bin')

        for dirpath, _, filenames in os.walk(path):
            for f in filenames:
                if f.lower().endswith(allowed_extensions):
                    fp = os.path.join(dirpath, f)
                    if os.path.isfile(fp):
                        total_size += os.path.getsize(fp)

        if total_size == 0:
            return "0 B"
        size_units = ["B", "KB", "MB", "GB", "TB"]
        i = int(math.floor(math.log(total_size, 1024)))
        p = math.pow(1024, i)
        s = round(total_size / p, 2)
        # print(f"Size: {s} {size_units[i]}")
        return f"{s} {size_units[i]}"

    def count_folders(self,path):
        return sum(
        1 for name in os.listdir(path)
        if os.path.isdir(os.path.join(path, name))
    )

    def count_total_folders(self,path):
        folder_count = 0
        for dirpath, dirnames, _ in os.walk(path):
            # Exclude the root folder itself from the count
            if dirpath != path:
                folder_count += 1
        return folder_count


def main():
    root = tk.Tk()
    LoginScreen(root)
    root.mainloop()


if __name__ == "__main__":
    main()