import tkinter as tk
from tkinter import ttk, PhotoImage
import time
import random
from PIL import Image, ImageTk, ImageDraw
import webbrowser
import math

class RoundedFrame(tk.Canvas):
    def __init__(self, master, radius=20, **kwargs):
        tk.Canvas.__init__(self, master, highlightthickness=0, **kwargs)
        self.radius = radius
        self.rect = None
        self.bind("<Configure>", self._draw_rounded_rect)
        
    def _draw_rounded_rect(self, event=None):
        width = self.winfo_width()
        height = self.winfo_height()
        
        self.delete("all")
        
        # Create rounded rectangle
        self.rect = self.round_rectangle(0, 0, width, height, radius=self.radius, fill=self["bg"])
        
    def round_rectangle(self, x1, y1, x2, y2, radius=25, **kwargs):
        points = []
        
        # Top left
        points.extend([x1 + radius, y1,
                      x1 + radius, y1,
                      x2 - radius, y1,
                      x2 - radius, y1])
        
        # Top right
        points.extend([x2, y1 + radius,
                      x2, y1 + radius,
                      x2, y2 - radius,
                      x2, y2 - radius])
        
        # Bottom right
        points.extend([x2 - radius, y2,
                      x2 - radius, y2,
                      x1 + radius, y2,
                      x1 + radius, y2])
        
        # Bottom left
        points.extend([x1, y2 - radius,
                      x1, y2 - radius,
                      x1, y1 + radius,
                      x1, y1 + radius])
        
        return self.create_polygon(points, **kwargs, smooth=True)

class SplashScreen:
    def __init__(self, root):
        self.root = root
        self.root.title("DB Vision")
        self.root.geometry("600x400")
        self.root.overrideredirect(True)
        
        # Center the splash screen
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        x = (screen_width - 600) // 2
        y = (screen_height - 400) // 2
        self.root.geometry(f"600x400+{x}+{y}")
        
        # Create gradient background with rounded corners
        self.canvas = tk.Canvas(root, width=600, height=400, highlightthickness=0, bg="#0f2027")
        self.canvas.pack()
        
        # Draw rounded rectangle for the main splash area
        self.canvas.create_round_rect(10, 10, 590, 390, radius=30, fill="", outline="#ffffff20", width=2)
        
        # Draw gradient
        self.draw_gradient(20, 20, 580, 380, "#0f2027", "#203a43", "#2c5364")
        
        # Add logo with modern font
        self.canvas.create_text(300, 150, text="DB Vision", 
                               font=("Segoe UI", 36, "bold"), fill="white")
        
        # Add loading text with animation dots
        self.loading_text = self.canvas.create_text(300, 250, text="Loading", 
                                                  font=("Segoe UI", 14), fill="white")
        self.dots = 0
        self.animate_loading()
        
        # Modern progress bar with rounded corners
        progress_bg = self.canvas.create_round_rect(150, 300, 450, 320, radius=10, fill="#ffffff20", outline="")
        self.progress_fg = self.canvas.create_round_rect(150, 300, 150, 320, radius=10, fill="#4a6fa5", outline="")
        
        # Add version info with subtle styling
        self.canvas.create_text(300, 370, text="Version 1.0.0", 
                              font=("Segoe UI", 8), fill="#ffffff80")
        
        # Simulate loading
        self.load_progress()
    
    def animate_loading(self):
        dots = "." * (self.dots % 4)
        self.canvas.itemconfig(self.loading_text, text=f"Loading{dots}")
        self.dots += 1
        if hasattr(self, "root") and self.root.winfo_exists():
            self.root.after(500, self.animate_loading)
    
    def draw_gradient(self, x1, y1, x2, y2, *colors):
        """Draw a gradient from top to bottom with rounded corners"""
        # Create a temporary image to draw the gradient
        width = x2 - x1
        height = y2 - y1
        gradient = Image.new("RGBA", (width, height))
        draw = ImageDraw.Draw(gradient)
        
        for i in range(height):
            ratio = i / height
            r = int(int(colors[0][1:3], 16) * (1 - ratio) + int(colors[-1][1:3], 16) * ratio)
            g = int(int(colors[0][3:5], 16) * (1 - ratio) + int(colors[-1][3:5], 16) * ratio)
            b = int(int(colors[0][5:7], 16) * (1 - ratio) + int(colors[-1][5:7], 16) * ratio)
            draw.line((0, i, width, i), fill=(r, g, b, 255))
        
        # Apply rounded corners to the gradient
        mask = Image.new("L", (width, height), 0)
        mask_draw = ImageDraw.Draw(mask)
        mask_draw.rounded_rectangle((0, 0, width, height), radius=20, fill=255)
        
        gradient.putalpha(mask)
        
        # Convert to PhotoImage and display on canvas
        self.gradient_img = ImageTk.PhotoImage(gradient)
        self.canvas.create_image(x1, y1, image=self.gradient_img, anchor="nw")
    
    def load_progress(self):
        for i in range(101):
            progress_width = 150 + (300 * i / 100)
            self.canvas.coords(self.progress_fg, 150, 300, progress_width, 320)
            self.root.update()
            time.sleep(0.03)
        self.root.destroy()

class ModernButton(tk.Canvas):
    def __init__(self, master, text, radius=10, color="#4a6fa5", hover_color="#3a5a80", 
                 text_color="white", command=None, width=120, height=40, font=("Segoe UI", 10)):
        tk.Canvas.__init__(self, master, width=width, height=height, highlightthickness=0)
        self.color = color
        self.hover_color = hover_color
        self.text_color = text_color
        self.command = command
        self.radius = radius
        self.font = font
        self.text = text
        
        self.bind("<Enter>", self.on_enter)
        self.bind("<Leave>", self.on_leave)
        self.bind("<Button-1>", self.on_click)
        
        self.draw_button()
    
    def draw_button(self, hover=False):
        self.delete("all")
        color = self.hover_color if hover else self.color
        
        # Draw rounded rectangle
        self.create_round_rect(0, 0, self.winfo_width(), self.winfo_height(), 
                              radius=self.radius, fill=color)
        
        # Add text
        self.create_text(self.winfo_width()/2, self.winfo_height()/2, 
                        text=self.text, fill=self.text_color, font=self.font)
    
    def on_enter(self, event):
        self.draw_button(hover=True)
    
    def on_leave(self, event):
        self.draw_button(hover=False)
    
    def on_click(self, event):
        if self.command:
            self.command()
    
    def create_round_rect(self, x1, y1, x2, y2, radius=25, **kwargs):
        points = []
        
        # Top left
        points.extend([x1 + radius, y1,
                      x1 + radius, y1,
                      x2 - radius, y1,
                      x2 - radius, y1])
        
        # Top right
        points.extend([x2, y1 + radius,
                      x2, y1 + radius,
                      x2, y2 - radius,
                      x2, y2 - radius])
        
        # Bottom right
        points.extend([x2 - radius, y2,
                      x2 - radius, y2,
                      x1 + radius, y2,
                      x1 + radius, y2])
        
        # Bottom left
        points.extend([x1, y2 - radius,
                      x1, y2 - radius,
                      x1, y1 + radius,
                      x1, y1 + radius])
        
        return self.create_polygon(points, **kwargs, smooth=True)

class DatabaseApp:
    def __init__(self, root):
        self.root = root
        self.root.title("DB Vision - Database Management System")
        self.root.geometry("1200x800")
        self.root.minsize(1000, 700)
        
        # Custom window styling
        self.root.configure(bg="#f8f9fa")
        self.style = ttk.Style()
        self.style.theme_use("clam")
        
        # Custom styles with rounded corners
        self.style.configure("TButton", padding=8, relief="flat", background="#4a6fa5", 
                           foreground="white", font=("Segoe UI", 10), borderwidth=0, 
                           focuscolor="none", focusthickness=0)
        self.style.map("TButton", 
                      background=[("active", "#3a5a80"), ("pressed", "#2a4a70")],
                      foreground=[("disabled", "#cccccc")])
        
        self.style.configure("TFrame", background="#f8f9fa")
        self.style.configure("TLabel", background="#f8f9fa", font=("Segoe UI", 10))
        self.style.configure("TEntry", fieldbackground="white", bordercolor="#ced4da", 
                           lightcolor="#ced4da", darkcolor="#ced4da", relief="flat", 
                           padding=5, font=("Segoe UI", 10))
        self.style.configure("Treeview", background="white", fieldbackground="white", 
                           rowheight=25, font=("Segoe UI", 10), bordercolor="#dee2e6")
        self.style.configure("Treeview.Heading", background="#e9ecef", relief="flat", 
                           font=("Segoe UI", 10, "bold"))
        self.style.map("Treeview", background=[("selected", "#4a6fa5")])
        
        # Create main container with subtle shadow effect
        self.main_container = RoundedFrame(root, radius=0, bg="#f8f9fa")
        self.main_container.pack(fill="both", expand=True, padx=10, pady=10)
        
        # Create drawer (left sidebar) with rounded top-right and bottom-right corners
        self.drawer = tk.Canvas(self.main_container, width=250, bg="#2c3e50", highlightthickness=0)
        self.drawer.pack(side="left", fill="y")
        
        # Draw rounded corners on drawer
        self.drawer.create_round_rect(0, 0, 250, 800, radius=20, fill="#2c3e50", outline="", corners=[False, True, True, False])
        
        # Drawer content
        self.drawer_content = tk.Frame(self.drawer, bg="#2c3e50")
        self.drawer.create_window((0, 20), window=self.drawer_content, anchor="nw", width=250)
        
        # Add logo to drawer with modern typography
        logo_label = tk.Label(self.drawer_content, text="DB Vision", bg="#2c3e50", fg="white", 
                             font=("Segoe UI", 20, "bold"), pady=30)
        logo_label.pack(fill="x")
        
        # Drawer menu items with modern icons and hover effects
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
            btn_frame = tk.Frame(self.drawer_content, bg="#2c3e50")
            btn_frame.pack(fill="x", padx=10, pady=2)
            
            btn = tk.Label(btn_frame, text=f"  {icon}  {text}", anchor="w", 
                          bg="#2c3e50", fg="white", font=("Segoe UI", 12), 
                          padx=15, pady=12, bd=0)
            btn.pack(fill="x")
            
            # Add rounded hover effect
            btn.bind("<Enter>", lambda e, b=btn: b.config(bg="#34495e"))
            btn.bind("<Leave>", lambda e, b=btn, t=text: 
                    b.config(bg="#34495e" if self.active_menu.get() == t else "#2c3e50"))
            btn.bind("<Button-1>", lambda e, t=text: self.set_active_menu(t))
            
            if text == "Dashboard":
                btn.config(bg="#34495e")
        
        # Add some space and then the bottom items
        tk.Frame(self.drawer_content, height=20, bg="#2c3e50").pack(fill="x")
        
        # Help button with modern style
        help_frame = tk.Frame(self.drawer_content, bg="#2c3e50")
        help_frame.pack(fill="x", side="bottom", padx=10, pady=10)
        
        help_btn = tk.Label(help_frame, text=" ℹ️  Help & Support", anchor="w", 
                           bg="#2c3e50", fg="white", font=("Segoe UI", 12), 
                           padx=15, pady=12)
        help_btn.pack(fill="x")
        
        help_btn.bind("<Enter>", lambda e: help_btn.config(bg="#34495e"))
        help_btn.bind("<Leave>", lambda e: help_btn.config(bg="#2c3e50"))
        help_btn.bind("<Button-1>", lambda e: self.open_help())
        
        # Create main content area
        self.main_content = ttk.Frame(self.main_container)
        self.main_content.pack(side="right", fill="both", expand=True)
        
        # Create top toolbar with rounded bottom corners
        self.toolbar = RoundedFrame(self.main_content, height=60, bg="#4a6fa5", radius=0)
        self.toolbar.pack(fill="x")
        self.toolbar.create_round_rect(0, 0, 1200, 60, radius=15, fill="#4a6fa5", outline="", corners=[False, False, True, True])
        
        # Connection status
        self.connection_status = tk.Label(self.toolbar, text="🟢 Connected to: localhost:27017", 
                                        bg="#4a6fa5", fg="white", font=("Segoe UI", 10))
        self.connection_status.place(x=20, y=20)
        
        # Spacer
        tk.Label(self.toolbar, text="", bg="#4a6fa5").pack(side="left", expand=True, fill="x")
        
        # User info with rounded avatar
        user_frame = tk.Frame(self.toolbar, bg="#4a6fa5")
        user_frame.pack(side="right", padx=20)
        
        # Circular avatar
        avatar_canvas = tk.Canvas(user_frame, width=32, height=32, bg="#4a6fa5", highlightthickness=0)
        avatar_canvas.pack(side="left", padx=(0, 10))
        avatar_canvas.create_oval(2, 2, 30, 30, fill="#3a5a80", outline="#ffffff80")
        avatar_canvas.create_text(16, 16, text="AU", fill="white", font=("Segoe UI", 10, "bold"))
        
        self.user_name = tk.Label(user_frame, text="Admin User", bg="#4a6fa5", 
                                fg="white", font=("Segoe UI", 10))
        self.user_name.pack(side="right")
        
        # Create content frame with subtle rounded corners
        self.content_frame = RoundedFrame(self.main_content, radius=15, bg="#f8f9fa")
        self.content_frame.pack(fill="both", expand=True, padx=20, pady=(0, 20))
        
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
            if isinstance(widget, tk.Frame):
                for child in widget.winfo_children():
                    if isinstance(child, tk.Label):
                        text = child.cget("text").strip().split()[-1]
                        if text == menu_name:
                            child.config(bg="#34495e")
                        else:
                            child.config(bg="#2c3e50")
        
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
        title = tk.Label(self.content_frame, text="Dashboard", font=("Segoe UI", 20, "bold"), bg="#f8f9fa")
        title.place(x=20, y=20)
        
        # Stats cards with rounded corners and subtle shadows
        stats_frame = tk.Frame(self.content_frame, bg="#f8f9fa")
        stats_frame.place(x=20, y=70, width=1160, height=120)
        
        stats = [
            {"title": "Databases", "value": "12", "color": "#3498db", "icon": "🗄️"},
            {"title": "Collections", "value": "84", "color": "#2ecc71", "icon": "📂"},
            {"title": "Documents", "value": "1.2M", "color": "#e74c3c", "icon": "📄"},
            {"title": "Indexes", "value": "56", "color": "#f39c12", "icon": "📈"}
        ]
        
        for i, stat in enumerate(stats):
            card = RoundedFrame(stats_frame, radius=15, bg="white")
            card.place(x=i*290, y=0, width=280, height=120)
            
            # Add subtle shadow effect
            card.create_round_rect(2, 2, 278, 118, radius=15, fill="#00000010", outline="")
            card.create_round_rect(0, 0, 280, 120, radius=15, fill="white", outline="#dee2e6")
            
            # Icon
            card.create_text(40, 40, text=stat["icon"], font=("Segoe UI", 24), fill=stat["color"])
            
            # Title and value
            card.create_text(80, 30, text=stat["title"], anchor="nw", 
                           font=("Segoe UI", 10), fill="#7f8c8d")
            card.create_text(80, 50, text=stat["value"], anchor="nw", 
                           font=("Segoe UI", 24, "bold"), fill=stat["color"])
        
        # Recent activity with rounded panels
        activity_frame = tk.Frame(self.content_frame, bg="#f8f9fa")
        activity_frame.place(x=20, y=210, width=1160, height=540)
        
        # Left panel - Performance with rounded corners
        perf_frame = RoundedFrame(activity_frame, radius=15, bg="white")
        perf_frame.place(x=0, y=0, width=570, height=540)
        
        # Add shadow and border
        perf_frame.create_round_rect(2, 2, 568, 538, radius=15, fill="#00000010", outline="")
        perf_frame.create_round_rect(0, 0, 570, 540, radius=15, fill="white", outline="#dee2e6")
        
        tk.Label(perf_frame, text="Performance Metrics", bg="white", 
                font=("Segoe UI", 12, "bold")).place(x=20, y=20)
        
        # Simulate a modern chart with area fill
        chart_canvas = tk.Canvas(perf_frame, bg="white", height=200, highlightthickness=0)
        chart_canvas.place(x=20, y=60, width=530)
        
        width = 530
        height = 200
        points = [(i * (width / 10), height - random.randint(20, 180)) for i in range(11)]
        
        # Create area fill
        area_points = [points[0][0], height]
        for x, y in points:
            area_points.extend([x, y])
        area_points.extend([points[-1][0], height])
        
        chart_canvas.create_polygon(area_points, fill="#3498db20", outline="")
        
        # Draw line
        for i in range(len(points) - 1):
            x1, y1 = points[i]
            x2, y2 = points[i + 1]
            chart_canvas.create_line(x1, y1, x2, y2, fill="#3498db", width=2, smooth=True)
        
        # Add circular data points
        for x, y in points:
            chart_canvas.create_oval(x-3, y-3, x+3, y+3, fill="#3498db", outline="")
        
        # Right panel - Recent queries with rounded corners
        queries_frame = RoundedFrame(activity_frame, radius=15, bg="white")
        queries_frame.place(x=590, y=0, width=570, height=540)
        
        # Add shadow and border
        queries_frame.create_round_rect(2, 2, 568, 538, radius=15, fill="#00000010", outline="")
        queries_frame.create_round_rect(0, 0, 570, 540, radius=15, fill="white", outline="#dee2e6")
        
        tk.Label(queries_frame, text="Recent Queries", bg="white", 
                font=("Segoe UI", 12, "bold")).place(x=20, y=20)
        
        queries = [
            {"query": "db.users.find({age: {$gt: 25}})", "time": "0.45s"},
            {"query": "db.products.aggregate([...])", "time": "1.23s"},
            {"query": "db.logs.countDocuments()", "time": "0.12s"},
            {"query": "db.orders.createIndex({date: 1})", "time": "2.10s"},
            {"query": "db.customers.updateMany(...)", "time": "0.87s"}
        ]
        
        for i, query in enumerate(queries):
            query_frame = tk.Frame(queries_frame, bg="white")
            query_frame.place(x=20, y=60 + i*50, width=530, height=40)
            
            # Rounded background for each query
            query_bg = tk.Canvas(query_frame, bg="white", highlightthickness=0)
            query_bg.pack(fill="both", expand=True)
            query_bg.create_round_rect(0, 0, 530, 40, radius=10, fill="#f8f9fa", outline="#dee2e6")
            
            tk.Label(query_bg, text=query["query"], bg="#f8f9fa", 
                    font=("Segoe UI", 9)).place(x=10, y=10)
            
            tk.Label(query_bg, text=query["time"], bg="#f8f9fa", 
                    fg="#7f8c8d", font=("Segoe UI", 9)).place(x=470, y=10)
    
    def show_databases(self):
        self.clear_content()
        
        title = tk.Label(self.content_frame, text="Databases", 
                        font=("Segoe UI", 20, "bold"), bg="#f8f9fa")
        title.place(x=20, y=20)
        
        # Search and action bar with rounded buttons
        action_frame = tk.Frame(self.content_frame, bg="#f8f9fa")
        action_frame.place(x=20, y=70, width=1160, height=50)
        
        # Modern search bar with rounded corners
        search_frame = RoundedFrame(action_frame, radius=20, bg="white")
        search_frame.place(x=0, y=0, width=300, height=40)
        
        search_entry = ttk.Entry(search_frame, style="Search.TEntry")
        search_entry.place(x=10, y=5, width=260, height=30)
        
        # Modern buttons with rounded corners
        search_btn = ModernButton(action_frame, "Search", width=100, height=40, 
                                color="#4a6fa5", hover_color="#3a5a80", 
                                command=lambda: print("Search clicked"))
        search_btn.place(x=320, y=0)
        
        create_btn = ModernButton(action_frame, "Create", width=100, height=40, 
                                color="#2ecc71", hover_color="#27ae60", 
                                command=lambda: print("Create clicked"))
        create_btn.place(x=430, y=0)
        
        refresh_btn = ModernButton(action_frame, "Refresh", width=100, height=40, 
                                 color="#95a5a6", hover_color="#7f8c8d", 
                                 command=lambda: print("Refresh clicked"))
        refresh_btn.place(x=540, y=0)
        
        # Database list with rounded corners
        db_frame = RoundedFrame(self.content_frame, radius=15, bg="white")
        db_frame.place(x=20, y=130, width=1160, height=620)
        
        # Add shadow and border
        db_frame.create_round_rect(2, 2, 1158, 618, radius=15, fill="#00000010", outline="")
        db_frame.create_round_rect(0, 0, 1160, 620, radius=15, fill="white", outline="#dee2e6")
        
        # Treeview for databases with custom style
        columns = ("name", "size", "collections", "indexes")
        tree = ttk.Treeview(db_frame, columns=columns, show="headings", style="Custom.Treeview")
        
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
        
        tree.place(x=10, y=10, width=1140, height=600)
        
        # Add scrollbar
        scrollbar = ttk.Scrollbar(db_frame, orient="vertical", command=tree.yview)
        scrollbar.place(x=1130, y=10, height=600)
        tree.configure(yscrollcommand=scrollbar.set)
    
    def show_collections(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Collections", 
                        font=("Segoe UI", 20, "bold"), bg="#f8f9fa")
        title.place(x=20, y=20)
        
        # Modern placeholder with rounded corners
        placeholder = RoundedFrame(self.content_frame, radius=15, bg="white")
        placeholder.place(x=20, y=70, width=1160, height=680)
        
        # Add shadow and border
        placeholder.create_round_rect(2, 2, 1158, 678, radius=15, fill="#00000010", outline="")
        placeholder.create_round_rect(0, 0, 1160, 680, radius=15, fill="white", outline="#dee2e6")
        
        # Placeholder content with modern styling
        tk.Label(placeholder, text="📂", font=("Segoe UI", 48), bg="white").place(x=560, y=250, anchor="center")
        tk.Label(placeholder, text="Collections Management", font=("Segoe UI", 16), bg="white").place(x=560, y=320, anchor="center")
        tk.Label(placeholder, text="This view will display all your database collections", 
                font=("Segoe UI", 12), bg="white", fg="#7f8c8d").place(x=560, y=360, anchor="center")
    
    def show_documents(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Documents", 
                        font=("Segoe UI", 20, "bold"), bg="#f8f9fa")
        title.place(x=20, y=20)
        
        # Modern placeholder with rounded corners
        placeholder = RoundedFrame(self.content_frame, radius=15, bg="white")
        placeholder.place(x=20, y=70, width=1160, height=680)
        
        # Add shadow and border
        placeholder.create_round_rect(2, 2, 1158, 678, radius=15, fill="#00000010", outline="")
        placeholder.create_round_rect(0, 0, 1160, 680, radius=15, fill="white", outline="#dee2e6")
        
        # Placeholder content with modern styling
        tk.Label(placeholder, text="📄", font=("Segoe UI", 48), bg="white").place(x=560, y=250, anchor="center")
        tk.Label(placeholder, text="Document Explorer", font=("Segoe UI", 16), bg="white").place(x=560, y=320, anchor="center")
        tk.Label(placeholder, text="Browse and edit your documents in this view", 
                font=("Segoe UI", 12), bg="white", fg="#7f8c8d").place(x=560, y=360, anchor="center")
    
    def show_query(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Query Builder", 
                        font=("Segoe UI", 20, "bold"), bg="#f8f9fa")
        title.place(x=20, y=20)
        
        # Modern placeholder with rounded corners
        placeholder = RoundedFrame(self.content_frame, radius=15, bg="white")
        placeholder.place(x=20, y=70, width=1160, height=680)
        
        # Add shadow and border
        placeholder.create_round_rect(2, 2, 1158, 678, radius=15, fill="#00000010", outline="")
        placeholder.create_round_rect(0, 0, 1160, 680, radius=15, fill="white", outline="#dee2e6")
        
        # Placeholder content with modern styling
        tk.Label(placeholder, text="🔍", font=("Segoe UI", 48), bg="white").place(x=560, y=250, anchor="center")
        tk.Label(placeholder, text="Query Builder", font=("Segoe UI", 16), bg="white").place(x=560, y=320, anchor="center")
        tk.Label(placeholder, text="Build and execute queries in this interactive view", 
                font=("Segoe UI", 12), bg="white", fg="#7f8c8d").place(x=560, y=360, anchor="center")
    
    def show_indexes(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Indexes", 
                        font=("Segoe UI", 20, "bold"), bg="#f8f9fa")
        title.place(x=20, y=20)
        
        # Modern placeholder with rounded corners
        placeholder = RoundedFrame(self.content_frame, radius=15, bg="white")
        placeholder.place(x=20, y=70, width=1160, height=680)
        
        # Add shadow and border
        placeholder.create_round_rect(2, 2, 1158, 678, radius=15, fill="#00000010", outline="")
        placeholder.create_round_rect(0, 0, 1160, 680, radius=15, fill="white", outline="#dee2e6")
        
        # Placeholder content with modern styling
        tk.Label(placeholder, text="📈", font=("Segoe UI", 48), bg="white").place(x=560, y=250, anchor="center")
        tk.Label(placeholder, text="Index Management", font=("Segoe UI", 16), bg="white").place(x=560, y=320, anchor="center")
        tk.Label(placeholder, text="View and manage your database indexes here", 
                font=("Segoe UI", 12), bg="white", fg="#7f8c8d").place(x=560, y=360, anchor="center")
    
        def show_users(self):
            self.clear_content()
            title = tk.Label(self.content_frame, text="Users", 
                            font=("Segoe UI", 20, "bold"), bg="#f8f9fa")
            title.place(x=20, y=20)
            
            # Modern placeholder with rounded corners
            placeholder = RoundedFrame(self.content_frame, radius=15, bg="white")
            placeholder.place(x=20, y=70, width=1160, height=680)
            
            # Add shadow and border
            placeholder.create_round_rect(2, 2, 1158, 678, radius=15, fill="#00000010", outline="")
            placeholder.create_round_rect(0, 0, 1160, 680, radius=15, fill="white", outline="#dee2e6")
            
            # Placeholder content with modern styling
            tk.Label(placeholder, text="👥", font=("Segoe UI", 48), bg="white").place(x=560, y=250, anchor="center")
            tk.Label(placeholder, text="User Management", font=("Segoe UI", 16), bg="white").place(x=560, y=320, anchor="center")
            tk.Label(placeholder, text="Manage database users and permissions in this view", 
                    font=("Segoe UI", 12), bg="white", fg="#7f8c8d").place(x=560, y=360, anchor="center")

    def show_settings(self):
        self.clear_content()
        title = tk.Label(self.content_frame, text="Settings", 
                        font=("Segoe UI", 20, "bold"), bg="#f8f9fa")
        title.place(x=20, y=20)
        
        # Modern placeholder with rounded corners
        placeholder = RoundedFrame(self.content_frame, radius=15, bg="white")
        placeholder.place(x=20, y=70, width=1160, height=680)
        
        # Add shadow and border
        placeholder.create_round_rect(2, 2, 1158, 678, radius=15, fill="#00000010", outline="")
        placeholder.create_round_rect(0, 0, 1160, 680, radius=15, fill="white", outline="#dee2e6")
        
        # Placeholder content with modern styling
        tk.Label(placeholder, text="⚙️", font=("Segoe UI", 48), bg="white").place(x=560, y=250, anchor="center")
        tk.Label(placeholder, text="Application Settings", font=("Segoe UI", 16), bg="white").place(x=560, y=320, anchor="center")
        tk.Label(placeholder, text="Configure your application preferences here", 
                font=("Segoe UI", 12), bg="white", fg="#7f8c8d").place(x=560, y=360, anchor="center")

    def open_help(self):
        webbrowser.open("https://example.com/db-vision-help")

# Main application execution
if __name__ == "__main__":
    root = tk.Tk()
    
    # Show splash screen first
    splash = SplashScreen(root)
    root.mainloop()
    
    # Then show main application
    root = tk.Tk()
    app = DatabaseApp(root)
    root.mainloop()