import csv
import random
import string

CSV_FILE = "data.csv"

# Generate random data
def generate_csv(filename, num_records=10000):
    with open(filename, mode="w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["name", "stName", "age", "something"])  # Header
        for _ in range(num_records):
            st_name = ''.join(random.choices(string.ascii_letters, k=10))
            age = random.randint(18, 80)
            gender = random.choice(["abc", "xyz"])
            writer.writerow([st_name, age, gender])

generate_csv(CSV_FILE)
print(f"Generated {CSV_FILE} with 10,000 records!")
