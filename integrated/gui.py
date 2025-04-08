import sys
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QWidget,
    QPushButton, QLabel, QLineEdit, QTextEdit, QTreeWidget, QTreeWidgetItem, QMessageBox, QSplitter
)
from PyQt5.QtCore import Qt

class LoginWindow(QMainWindow):
<<<<<<< HEAD
    def _init_(self):
        super()._init_()
=======
    def __init__(self):
        super().__init__()
>>>>>>> ed4a050a7c3edcbc6a467de7eb28ae540ae7a49c
        self.setWindowTitle("Login")
        self.setGeometry(100, 100, 400, 200)

        # Main layout
        self.central_widget = QWidget()
        self.layout = QVBoxLayout()

        # ID Input
        self.id_label = QLabel("ID:")
        self.id_input = QLineEdit()
        self.layout.addWidget(self.id_label)
        self.layout.addWidget(self.id_input)

        # Password Input
        self.password_label = QLabel("Password:")
        self.password_input = QLineEdit()
        self.password_input.setEchoMode(QLineEdit.Password)
        self.layout.addWidget(self.password_label)
        self.layout.addWidget(self.password_input)

        # Login Button
        self.login_button = QPushButton("Login")
        self.login_button.clicked.connect(self.verify_credentials)
        self.layout.addWidget(self.login_button)

        self.central_widget.setLayout(self.layout)
        self.setCentralWidget(self.central_widget)

    def verify_credentials(self):
        user_id = self.id_input.text()
        password = self.password_input.text()

        try:
            with open("details.txt", "r") as file:
                credentials = file.readlines()
                for line in credentials:
                    stored_id, stored_password = line.strip().split(",")
                    if user_id == stored_id and password == stored_password:
                        self.open_main_gui()
                        return
        except FileNotFoundError:
            QMessageBox.critical(self, "Error", "details.txt file not found!")
            return

        QMessageBox.critical(self, "Error", "Invalid ID or Password!")

    def open_main_gui(self):
        self.main_gui = DatabaseGUI()
        self.main_gui.show()
        self.close()


class DatabaseGUI(QMainWindow):
<<<<<<< HEAD
    def _init_(self):
        super()._init_()
=======
    def __init__(self):
        super().__init__()
>>>>>>> ed4a050a7c3edcbc6a467de7eb28ae540ae7a49c
        self.setWindowTitle("Nimbus Database GUI")
        self.setGeometry(100, 100, 1000, 600)

        # Main layout with splitter for left and right panels
        self.main_layout = QSplitter(Qt.Horizontal)

        # Left panel: Database and Table Tree
        self.db_tree = QTreeWidget()
        self.db_tree.setHeaderLabels(["Databases and Tables"])
        self.db_tree.itemClicked.connect(self.on_tree_item_clicked)
        self.main_layout.addWidget(self.db_tree)

        # Right panel: Controls and Output
        self.right_panel = QWidget()
        self.right_layout = QVBoxLayout()

        # Database Controls
        self.db_controls_layout = QHBoxLayout()
        self.db_name_input = QLineEdit()
        self.db_name_input.setPlaceholderText("Enter Database Name")
        self.create_db_btn = QPushButton("Create Database")
        self.load_db_btn = QPushButton("Load Database")
        self.db_controls_layout.addWidget(self.db_name_input)
        self.db_controls_layout.addWidget(self.create_db_btn)
        self.db_controls_layout.addWidget(self.load_db_btn)

        # Table Controls
        self.table_controls_layout = QHBoxLayout()
        self.table_name_input = QLineEdit()
        self.table_name_input.setPlaceholderText("Enter Table Name")
        self.create_table_btn = QPushButton("Create Table")
        self.drop_table_btn = QPushButton("Drop Table")
        self.table_controls_layout.addWidget(self.table_name_input)
        self.table_controls_layout.addWidget(self.create_table_btn)
        self.table_controls_layout.addWidget(self.drop_table_btn)

        # Query Section
        self.query_layout = QVBoxLayout()
        self.query_input = QTextEdit()
        self.query_input.setPlaceholderText("Enter Query (e.g., Insert, Search, etc.)")
        self.run_query_btn = QPushButton("Run Query")
        self.query_layout.addWidget(self.query_input)
        self.query_layout.addWidget(self.run_query_btn)

        # Output Section
        self.output_layout = QVBoxLayout()
        self.output_label = QLabel("Output:")
        self.output_text = QTextEdit()
        self.output_text.setReadOnly(True)
        self.output_layout.addWidget(self.output_label)
        self.output_layout.addWidget(self.output_text)

        # Add all layouts to the right panel
        self.right_layout.addLayout(self.db_controls_layout)
        self.right_layout.addLayout(self.table_controls_layout)
        self.right_layout.addLayout(self.query_layout)
        self.right_layout.addLayout(self.output_layout)
        self.right_panel.setLayout(self.right_layout)

        # Add right panel to the splitter
        self.main_layout.addWidget(self.right_panel)

        # Set central widget
        self.setCentralWidget(self.main_layout)

        # Connect buttons to functions
        self.create_db_btn.clicked.connect(self.create_database)
        self.load_db_btn.clicked.connect(self.load_database)
        self.create_table_btn.clicked.connect(self.create_table)
        self.drop_table_btn.clicked.connect(self.drop_table)
        self.run_query_btn.clicked.connect(self.run_query)

        # Mock data for demonstration
        self.databases = {}  # Format: {db_name: {table_name: schema}}

    def create_database(self):
        db_name = self.db_name_input.text()
        if not db_name:
            self.show_error("Database name cannot be empty!")
            return
        if db_name in self.databases:
            self.show_error("Database already exists!")
            return
        self.databases[db_name] = {}
        self.update_db_tree()
        self.output_text.append(f"Database '{db_name}' created successfully!")

    def load_database(self):
        db_name = self.db_name_input.text()
        if not db_name:
            self.show_error("Database name cannot be empty!")
            return
        # Simulate loading database (in real implementation, load from backend)
        if db_name not in self.databases:
            self.databases[db_name] = {}
        self.update_db_tree()
        self.output_text.append(f"Database '{db_name}' loaded successfully!")

    def create_table(self):
        db_name = self.db_name_input.text()
        table_name = self.table_name_input.text()
        if not db_name or not table_name:
            self.show_error("Database and Table names cannot be empty!")
            return
        if db_name not in self.databases:
            self.show_error("Database does not exist!")
            return
        if table_name in self.databases[db_name]:
            self.show_error("Table already exists in the database!")
            return
        # Simulate table schema (in real implementation, get schema from backend)
        schema = ["id INT", "name STRING", "email STRING"]
        self.databases[db_name][table_name] = schema
        self.update_db_tree()
        self.output_text.append(f"Table '{table_name}' created successfully in database '{db_name}'!")

    def drop_table(self):
        db_name = self.db_name_input.text()
        table_name = self.table_name_input.text()
        if not db_name or not table_name:
            self.show_error("Database and Table names cannot be empty!")
            return
        if db_name not in self.databases or table_name not in self.databases[db_name]:
            self.show_error("Table does not exist in the database!")
            return
        del self.databases[db_name][table_name]
        self.update_db_tree()
        self.output_text.append(f"Table '{table_name}' dropped successfully from database '{db_name}'!")

    def run_query(self):
        query = self.query_input.toPlainText()
        if not query:
            self.show_error("Query cannot be empty!")
            return
        # Call your C++ backend function here
        # Example: database.runQuery(query)
        self.output_text.append(f"Query executed: {query}")

    def update_db_tree(self):
        """Update the database and table tree in the left panel."""
        self.db_tree.clear()
        for db_name, tables in self.databases.items():
            db_item = QTreeWidgetItem([db_name])
            for table_name, schema in tables.items():
                table_item = QTreeWidgetItem([table_name])
                for column in schema:
                    schema_item = QTreeWidgetItem([column])
                    table_item.addChild(schema_item)
                db_item.addChild(table_item)
            self.db_tree.addTopLevelItem(db_item)

    def on_tree_item_clicked(self, item):
        """Handle clicks on the tree items."""
        parent = item.parent()
        if parent is None:
            # Top-level item (database)
            db_name = item.text(0)
            self.output_text.append(f"Selected Database: {db_name}")
        else:
            # Child item (table or schema)
            table_name = item.text(0)
            db_name = parent.text(0)
            if parent.parent() is None:
                # Table level
                self.output_text.append(f"Selected Table: {table_name} in Database: {db_name}")
            else:
                # Schema level
                self.output_text.append(f"Selected Schema: {table_name} in Table: {parent.text(0)} of Database: {db_name}")

    def show_error(self, message):
        QMessageBox.critical(self, "Error", message)


<<<<<<< HEAD
if _name_ == "_main_":
=======
if __name__ == "__main__":
>>>>>>> ed4a050a7c3edcbc6a467de7eb28ae540ae7a49c
    app = QApplication(sys.argv)
    login_window = LoginWindow()  # Start with the LoginWindow
    login_window.show()
    sys.exit(app.exec_())