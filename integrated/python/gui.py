import tkinter as tk
from tkinter import ttk, PhotoImage
import time
import random
from PIL import Image, ImageTk
import webbrowser

class SplashScreen:
    def __init__(self, root):
        self.root = root
        self.root.title("Nimbus")
        self.root.geometry("600x400")
        self.root.overrideredirect(True)  # Remove window decorations
        
        # Center the splash screen
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        x = (screen_width - 600) // 2
        y = (screen_height - 400) // 2
        self.root.geometry(f"600x400+{x}+{y}")
        
        # Create gradient background
        self.canvas = tk.Canvas(root, width=600, height=400, highlightthickness=0)
        self.canvas.pack()
        self.draw_gradient(0, 0, 600, 400, "#0f2027", "#203a43", "#2c5364")
        
        # Add logo (using text as placeholder)
        self.canvas.create_text(300, 150, text="DB Vision", font=("Helvetica", 36, "bold"), fill="white")
        
        # Add loading text
        self.loading_text = self.canvas.create_text(300, 250, text="Loading...", font=("Helvetica", 14), fill="white")
        
        # Add progress bar
        self.progress = ttk.Progressbar(root, orient="horizontal", length=300, mode="determinate")
        self.progress.place(x=150, y=300)
        
        # Add version info
        self.canvas.create_text(300, 380, text="Version 1.0.0", font=("Helvetica", 8), fill="white")
        
        # Simulate loading
        self.load_progress()
    
    def draw_gradient(self, x1, y1, x2, y2, *colors):
        """Draw a gradient from top to bottom"""
        for i in range(y1, y2):
            ratio = (i - y1) / (y2 - y1)
            r = int(int(colors[0][1:3], 16) * (1 - ratio) + int(colors[-1][1:3], 16) * ratio)
            g = int(int(colors[0][3:5], 16) * (1 - ratio) + int(colors[-1][3:5], 16) * ratio)
            b = int(int(colors[0][5:7], 16) * (1 - ratio) + int(colors[-1][5:7], 16) * ratio)
            color = f"#{r:02x}{g:02x}{b:02x}"
            self.canvas.create_line(x1, i, x2, i, fill=color)
    
    def load_progress(self):
        for i in range(101):
            self.progress["value"] = i
            self.root.update()
            time.sleep(0.03)  # Simulate loading time
        self.root.destroy()

class DatabaseApp:
    def __init__(self, root):
        self.root = root
        self.root.title("DB Vision - Database Management System")
        self.root.geometry("1200x800")
        self.root.minsize(1000, 700)
        
        # Custom window styling
        self.root.configure(bg="#f5f5f5")
        self.style = ttk.Style()
        self.style.theme_use("clam")
        
        # Custom styles
        self.style.configure("TButton", padding=6, relief="flat", background="#4a6fa5", foreground="white")
        self.style.map("TButton", background=[("active", "#3a5a80")])
        self.style.configure("TFrame", background="#f5f5f5")
        self.style.configure("TLabel", background="#f5f5f5", font=("Helvetica", 10))
        self.style.configure("TEntry", fieldbackground="white")
        self.style.configure("Treeview", background="white", fieldbackground="white")
        
        # Create main container
        self.main_container = ttk.Frame(root)
        self.main_container.pack(fill="both", expand=True)
        
        # Create drawer (left sidebar)
        self.drawer = tk.Canvas(self.main_container, width=250, bg="#2c3e50", highlightthickness=0)
        self.drawer.pack(side="left", fill="y")
        
        # Drawer content
        self.drawer_content = tk.Frame(self.drawer, bg="#2c3e50")
        self.drawer.create_window((0, 0), window=self.drawer_content, anchor="nw", width=250)
        
        # Add logo to drawer
        logo_label = tk.Label(self.drawer_content, text="DB Vision", bg="#2c3e50", fg="white", 
                             font=("Helvetica", 18, "bold"), pady=20)
        logo_label.pack(fill="x")
        
        # Drawer menu items
        menu_items = [
            ("Dashboard", "📊"),
            ("Databases", "🗄️"),
            ("Collections", "📂"),
            ("Documents", "📄"),
            ("Query", "🔍"),
            ("Indexes", "📈"),
            ("Users", "👥"),
            ("Settings", "⚙️")
        ]
        
        self.active_menu = tk.StringVar(value="Dashboard")
        
        for text, icon in menu_items:
            btn = tk.Button(self.drawer_content, text=f" {icon}  {text}", anchor="w", 
                          bg="#2c3e50", fg="white", font=("Helvetica", 12), bd=0,
                          padx=20, pady=12, command=lambda t=text: self.set_active_menu(t))
            btn.pack(fill="x")
            
            if text == "Dashboard":
                btn.config(bg="#34495e")
        
        # Add some space and then the bottom items
        tk.Frame(self.drawer_content, height=20, bg="#2c3e50").pack(fill="x")
        
        # Help button
        help_btn = tk.Button(self.drawer_content, text=" ℹ️  Help & Support", anchor="w", 
                            bg="#2c3e50", fg="white", font=("Helvetica", 12), bd=0,
                            padx=20, pady=12, command=self.open_help)
        help_btn.pack(fill="x", side="bottom")
        
        # Create main content area
        self.main_content = ttk.Frame(self.main_container)
        self.main_content.pack(side="right", fill="both", expand=True)
        
        # Create top toolbar
        self.toolbar = ttk.Frame(self.main_content, height=60)
        self.toolbar.pack(fill="x")
        
        # Toolbar content
        self.toolbar.config(style="Toolbar.TFrame")
        self.style.configure("Toolbar.TFrame", background="#4a6fa5")
        
        # Connection status
        self.connection_status = tk.Label(self.toolbar, text="Connected to: localhost:27017", 
                                        bg="#4a6fa5", fg="white", font=("Helvetica", 10))
        self.connection_status.pack(side="left", padx=20)
        
        # Spacer
        tk.Label(self.toolbar, text="", bg="#4a6fa5").pack(side="left", expand=True, fill="x")
        
        # User info
        user_frame = tk.Frame(self.toolbar, bg="#4a6fa5")
        user_frame.pack(side="right", padx=10)
        
        # User avatar (placeholder)
        self.user_avatar = tk.Label(user_frame, text="👤", bg="#4a6fa5", fg="white", font=("Helvetica", 14))
        self.user_avatar.pack(side="left", padx=(0, 10))
        
        self.user_name = tk.Label(user_frame, text="Admin User", bg="#4a6fa5", fg="white", font=("Helvetica", 10))
        self.user_name.pack(side="right")
        
        # Create content frame
        self.content_frame = ttk.Frame(self.main_content)
        self.content_frame.pack(fill="both", expand=True, padx=20, pady=20)
        
        # Initialize dashboard
        self.show_dashboard()
        
        # Configure drawer scrolling
        self.drawer.bind("<Configure>", lambda e: self.drawer.configure(scrollregion=self.drawer.bbox("all")))
        
        # Add scrollbar to drawer
        self.drawer_scroll = ttk.Scrollbar(self.main_container, orient="vertical", command=self.drawer.yview)
        self.drawer_scroll.pack(side="left", fill="y")
        self.drawer.configure(yscrollcommand=self.drawer_scroll.set)
        
        # Bind mouse wheel to drawer
        self.drawer.bind_all("<MouseWheel>", lambda e: self.drawer.yview_scroll(int(-1*(e.delta/120)), "units"))
        
        # Update drawer scrollregion
        self.drawer_content.update_idletasks()
        self.drawer.configure(scrollregion=self.drawer.bbox("all"))
    
    def set_active_menu(self, menu_name):
        self.active_menu.set(menu_name)
        
        # Update button colors
        for widget in self.drawer_content.winfo_children():
            if isinstance(widget, tk.Button) and widget.cget("text").strip().endswith(menu_name):
                widget.config(bg="#34495e")
            elif isinstance(widget, tk.Button):
                widget.config(bg="#2c3e50")
        
        # Show appropriate content
        if menu_name == "Dashboard":
            self.show_dashboard()
        elif menu_name == "Databases":
            self.show_databases()
        elif menu_name == "Collections":
            self.show_collections()
        elif menu_name == "Documents":
            self.show_documents()
        elif menu_name == "Query":
            self.show_query()
        elif menu_name == "Indexes":
            self.show_indexes()
        elif menu_name == "Users":
            self.show_users()
        elif menu_name == "Settings":
            self.show_settings()
    
    def clear_content(self):
        for widget in self.content_frame.winfo_children():
            widget.destroy()
    
    def show_dashboard(self):
        self.clear_content()
        
        # Dashboard title
        title = tk.Label(self.content_frame, text="Dashboard", font=("Helvetica", 20, "bold"), bg="#f5f5f5")
        title.pack(anchor="nw", pady=(0, 20))
        
        # Stats cards
        stats_frame = tk.Frame(self.content_frame, bg="#f5f5f5")
        stats_frame.pack(fill="x", pady=(0, 20))
        
        stats = [
            {"title": "Databases", "value": "12", "color": "#3498db"},
            {"title": "Collections", "value": "84", "color": "#2ecc71"},
            {"title": "Documents", "value": "1.2M", "color": "#e74c3c"},
            {"title": "Indexes", "value": "56", "color": "#f39c12"}
        ]
        
        for stat in stats:
            card = tk.Frame(stats_frame, bg="white", padx=20, pady=15, relief="groove", bd=1)
            card.pack(side="left", fill="x", expand=True, padx=(0, 15))
            
            tk.Label(card, text=stat["title"], bg="white", fg="#7f8c8d", font=("Helvetica", 10)).pack(anchor="nw")
            tk.Label(card, text=stat["value"], bg="white", fg=stat["color"], font=("Helvetica", 24, "bold")).pack(anchor="nw")
        
        # Recent activity
        activity_frame = tk.Frame(self.content_frame, bg="#f5f5f5")
        activity_frame.pack(fill="both", expand=True)
        
        # Left panel - Performance
        perf_frame = tk.Frame(activity_frame, bg="white", padx=15, pady=15, relief="groove", bd=1)
        perf_frame.pack(side="left", fill="both", expand=True, padx=(0, 10))
        
        tk.Label(perf_frame, text="Performance Metrics", bg="white", font=("Helvetica", 12, "bold")).pack(anchor="nw", pady=(0, 10))
        
        # Simulate a chart with canvas
        chart_canvas = tk.Canvas(perf_frame, bg="white", height=200, highlightthickness=0)
        chart_canvas.pack(fill="x", pady=10)
        
        # Draw a simple line chart
        width = 400
        height = 200
        points = [(i * (width / 10), height - random.randint(20, 180)) for i in range(11)]
        
        for i in range(len(points) - 1):
            x1, y1 = points[i]
            x2, y2 = points[i + 1]
            chart_canvas.create_line(x1, y1, x2, y2, fill="#3498db", width=2)
        
        # Right panel - Recent queries
        queries_frame = tk.Frame(activity_frame, bg="white", padx=15, pady=15, relief="groove", bd=1)
        queries_frame.pack(side="right", fill="both", expand=True)
        
        tk.Label(queries_frame, text="Recent Queries", bg="white", font=("Helvetica", 12, "bold")).pack(anchor="nw", pady=(0, 10))
        
        queries = [
            {"query": "db.users.find({age: {$gt: 25}})", "time": "0.45s"},
            {"query": "db.products.aggregate([...])", "time": "1.23s"},
            {"query": "db.logs.countDocuments()", "time": "0.12s"},
            {"query": "db.orders.createIndex({date: 1})", "time": "2.10s"},
            {"query": "db.customers.updateMany(...)", "time": "0.87s"}
        ]
        
        for query in queries:
            frame = tk.Frame(queries_frame, bg="white")
            frame.pack(fill="x", pady=5)
            
            tk.Label(frame, text=query["query"], bg="white", font=("Helvetica", 9)).pack(side="left")
            tk.Label(frame, text=query["time"], bg="white", fg="#7f8c8d", font=("Helvetica", 9)).pack(side="right")
    
    def show_databases(self):
        self.clear_content()
        
        title = tk.Label(self.content_frame, text="Databases", font=("Helvetica", 20, "bold"), bg="#f5f5f5")
        title.pack(anchor="nw", pady=(0, 20))
        
        # Search and action bar
        action_frame = tk.Frame(self.content_frame, bg="#f5f5f5")
        action_frame.pack(fill="x", pady=(0, 20))
        
        search_entry = ttk.Entry(action_frame, width=40)
        search_entry.pack(side="left", padx=(0, 10))
        
        ttk.Button(action_frame, text="Search").pack(side="left", padx=(0, 10))
        ttk.Button(action_frame, text="Create Database").pack(side="left", padx=(0, 10))
        ttk.Button(action_frame, text="Refresh").pack(side="left")
        
        # Database list
        db_frame = tk.Frame(self.content_frame, bg="white", relief="groove", bd=1)
        db_frame.pack(fill="both", expand=True)
        
        # Treeview for databases
        columns = ("name", "size", "collections", "indexes")
        tree = ttk.Treeview(db_frame, columns=columns, show="headings")
        
        tree.heading("name", text="Database Name")
        tree.heading("size", text="Size")
        tree.heading("collections", text="Collections")
        tree.heading("indexes", text="Indexes")
        
        tree.column("name", width=300)
        tree.column("size", width=150, anchor="center")
        tree.column("collections", width=100, anchor="center")
        tree.column("indexes", width=100, anchor="center")
        
        # Sample data
        databases = [
            ("admin", "12.4 MB", "4", "8"),
            ("config", "8.2 MB", "3", "5"),
            ("local", "5.7 MB", "2", "3"),
            ("ecommerce", "1.2 GB", "24", "32"),
            ("analytics", "845.3 MB", "18", "25"),
            ("users", "324.7 MB", "12", "15"),
            ("logs", "2.4 GB", "8", "12")
        ]
        
        for db in databases:
            tree.insert("", "end", values=db)
        
        tree.pack(fill="both", expand=True, padx=5, pady=5)
    
    def show_collections(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Collections", font=("Helvetica", 20, "bold"), bg="#f5f5f5")
        title.pack(anchor="nw", pady=(0, 20))
        
        # Placeholder content
        placeholder = tk.Label(self.content_frame, text="Collections management view will be displayed here", 
                             font=("Helvetica", 12), bg="#f5f5f5")
        placeholder.pack(fill="both", expand=True)
    
    def show_documents(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Documents", font=("Helvetica", 20, "bold"), bg="#f5f5f5")
        title.pack(anchor="nw", pady=(0, 20))
        
        # Placeholder content
        placeholder = tk.Label(self.content_frame, text="Document explorer and editor will be displayed here", 
                             font=("Helvetica", 12), bg="#f5f5f5")
        placeholder.pack(fill="both", expand=True)
    
    def show_query(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Query", font=("Helvetica", 20, "bold"), bg="#f5f5f5")
        title.pack(anchor="nw", pady=(0, 20))
        
        # Placeholder content
        placeholder = tk.Label(self.content_frame, text="Query builder and results viewer will be displayed here", 
                             font=("Helvetica", 12), bg="#f5f5f5")
        placeholder.pack(fill="both", expand=True)
    
    def show_indexes(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Indexes", font=("Helvetica", 20, "bold"), bg="#f5f5f5")
        title.pack(anchor="nw", pady=(0, 20))
        
        # Placeholder content
        placeholder = tk.Label(self.content_frame, text="Index management view will be displayed here", 
                             font=("Helvetica", 12), bg="#f5f5f5")
        placeholder.pack(fill="both", expand=True)
    
    def show_users(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Users", font=("Helvetica", 20, "bold"), bg="#f5f5f5")
        title.pack(anchor="nw", pady=(0, 20))
        
        # Placeholder content
        placeholder = tk.Label(self.content_frame, text="User management view will be displayed here", 
                             font=("Helvetica", 12), bg="#f5f5f5")
        placeholder.pack(fill="both", expand=True)
    
    def show_settings(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Settings", font=("Helvetica", 20, "bold"), bg="#f5f5f5")
        title.pack(anchor="nw", pady=(0, 20))
        
        # Placeholder content
        placeholder = tk.Label(self.content_frame, text="Application settings will be displayed here", 
                             font=("Helvetica", 12), bg="#f5f5f5")
        placeholder.pack(fill="both", expand=True)
    
    def open_help(self):
        webbrowser.open("https://example.com/db-vision-help")

def main():
    # Create splash screen
    splash_root = tk.Tk()
    splash = SplashScreen(splash_root)
    splash_root.mainloop()
    
    # Create main application
    root = tk.Tk()
    app = DatabaseApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()

